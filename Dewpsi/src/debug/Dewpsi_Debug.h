#ifndef DEWPSI_DEBUG_H
#define DEWPSI_DEBUG_H

/**
*   @file       Dewpsi_Debug.h
*   @brief      @doxfb
*   Contains debug macros.
*   @defgroup debug Debugging
*   Debugging macros and classes.
*   @ingroup core
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Timer.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>

/**
*   @def        _PD_DEBUG_BREAK()
*   @brief      Inserts a breakpoint at the line it was called.
*   @par        Conditions
*   Define @c PD_DEBUG and @c _PD_DEBUG_BREAKS prior to including this header.
*   Set the external variable @a g_bDewpsiBreaks to true.
*   @ingroup    debug
*/
#if defined(PD_DEBUG)
extern bool g_bDewpsiBreaks;
    // if defined: _PD_DEBUG_DEFINE_VARS
    #if defined(_PD_DEBUG_DEFINE_VARS)
        #warning "g_bDewpsiBreaks exported as symbol, initialized to false"
bool g_bDewpsiBreaks = false;
    #endif

    // if defined: _PD_DEBUG_BREAKS
    #if defined(_PD_DEBUG_BREAKS)
        #define _PD_DEBUG_BREAK()   if (g_bDewpsiBreaks) {\
                                        ::std::cerr << "Break at " __FILE__ ", line " << __LINE__ << '\n'; \
                                        PD_DEBUGBREAK(); \
                                    }
    #else
        #define _PD_DEBUG_BREAK()
    #endif
#else
    #define _PD_DEBUG_BREAK()
#endif /* PD_DEBUG defined */

#if defined(_PD_DEBUG_DEFINE_VARS)
    #undef _PD_DEBUG_DEFINE_VARS
#endif

#if defined(_PD_DEBUG_BREAKS)
    #undef _PD_DEBUG_BREAKS
#endif

#ifndef PD_PROFILE
    #define PD_PROFILE 0
#endif

/** Enables debug breakpoints for this scope.
*   Instantiates an object of type @c %DebugBreakEnabler named @a n.
*   @param      n       The name of an object to create
*   @ingroup    debug
*/
#define PD_ENABLE_BREAKS(n)     ::Dewpsi::DebugBreakEnabler n(__FILE__,__LINE__)

#ifdef PD_DEBUG
    /** Creates a breakpoint object.
    *   @ingroup debug
    */
    #define PD_CREATE_BREAKER() ::Dewpsi::DebugBreaker _DEBUG_BREAKER(__FILE__,__LINE__);
    /** Executes the breakpoint defined with PD_CREATE_BREAKER().
    *   @ingroup debug
    */
    #define PD_BREAKER_EXEC()   _DEBUG_BREAKER.BreakHere(__LINE__);
#endif

namespace Dewpsi {
    using FloatMicroSeconds = std::chrono::duration<double, std::micro>;

    /** Enables and disables debugging at the scope level.
    *   @ingroup debug
    */
    class DebugBreakEnabler {
    public:
        /// Enable debug breaks.
        DebugBreakEnabler(const char* file, int line)
            : m_cpFile(file), m_iLine(line)
        {
#ifdef PD_DEBUG
            g_bDewpsiBreaks = true;
#endif
            PD_CORE_TRACE("Enabled debug breaks for {0}:{1}", file, line);
        }

        /// Disable debug breaks.
        ~DebugBreakEnabler()
        {
#ifdef PD_DEBUG
            g_bDewpsiBreaks = false;
#endif
            m_cpFile = nullptr;
            m_iLine = 0;
        }

    private:
        const char* m_cpFile;
        int m_iLine;
    };

#ifdef PD_DEBUG
    class DebugBreaker {
    public:
        DebugBreaker(const char* file, int line)
            : m_File(file), m_Line(line)
        {
            PD_CORE_TRACE("Debug breaker defined at {0}:{1}", m_File, m_Line);
        }

        void BreakHere(int line)
        {
            PD_CORE_TRACE("Breakpoint at {0}:{1}", m_File, line-m_Line);
            std::raise(SIGINT);
        }

    private:
        const char* m_File;
        int m_Line;
    };
#endif

    //////////////////////////////////////////////////////////
    // Instrumentation ////////////////////////////////////////
    //////////////////////////////////////////////////////////

    /// @addtogroup debug
    /// @{

    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

    /// Result of the profile.
	struct ProfileResult
	{
		PDstring name;
		FloatingPointMicroseconds start;
		std::chrono::microseconds elapsedTime;
		std::thread::id threadID;
	};

    /// Instrumentation session.
    struct InstrumentationSession {
        PDstring name;
    };

    /// Instrumentation class: writes JSON output to a file.
    class Instrumentation {
    public:
        Instrumentation(const Instrumentation&) = delete;
		Instrumentation(Instrumentation&&) = delete;

        /// Open a file and begin a session.
        void BeginSession(const PDstring& name, const PDstring& file = "results.json")
        {
            // close existing session
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_Session)
            {
                if (Log::GetCoreLogger())
                {
                    PD_CORE_ERROR("Instrumentation: opening session '{}' when old session '{}' not closed",
                                  name, m_Session->name);
                }
                InternalEndSession();
            }
            m_OutputStream.open(file);

            // new session
            if (m_OutputStream.is_open())
            {
                m_Session = new InstrumentationSession({name});
                WriteHeader();
            }
            else
            {
                if (Log::GetCoreLogger())
                {
                    PD_CORE_ERROR("Instrumentation failed to open '{0}'", file);
                }
            }
        }

        /// End the session.
        void EndSession()
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			InternalEndSession();
		}

        /// Write a profile to the file.
        void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.elapsedTime.count()) << ',';
			json << "\"name\":\"" << result.name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.threadID << ",";
			json << "\"ts\":" << result.start.count();
			json << "}";

			std::lock_guard<std::mutex> lock(m_Mutex);
			if (m_Session)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

        /// Retrieve an instance of @doxtype{Instrumentation}.
		static Instrumentation& Get()
		{
			static Instrumentation instance;
			return instance;
		}

    private:
        Instrumentation() : m_Mutex(), m_Session(), m_OutputStream() {}
        ~Instrumentation() {EndSession();}

        void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

        void InternalEndSession()
		{
			if (m_Session)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_Session;
				m_Session = nullptr;
			}
		}

        std::mutex m_Mutex;
        InstrumentationSession* m_Session;
        std::ofstream m_OutputStream;
    };

    /** A timer that measures the time elapsed in a particular scope (ie, a function).
    *   As soon as it is constructed, the timer stops; and when it is destroyed, the
    *   timer is stopped and the time elapsed is logged.
    */
    class InstrumentationTimer : public ScopeTimer {
    public:
        /// Starts the time named for the scope @a name.
        InstrumentationTimer(const char* name) : m_Name(name) {}

        /// Stops the timer and prints the elapsed time.
        ~InstrumentationTimer() {
            Stop();
            Print();
        }

        /// Prints the time elapsed in milliseconds.
        void Print()
        {
            auto highResStart = FloatingPointMicroseconds(m_Start.time_since_epoch());
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(m_Elapsed);
            ProfileResult result = { m_Name, highResStart, elapsedTime, std::this_thread::get_id() };
            Instrumentation::Get().WriteProfile(result);
        }

    private:
        const char* m_Name;
    };

    /// @}
} // End namespace Dewpsi

#ifndef PD_PROFILE
#define PD_PROFILE 0
#endif

#if PD_PROFILE
    #warning "Including PD_PROFILE macros"

    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define PD_FUNCTION_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define PD_FUNCTION_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define PD_FUNCTION_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define PD_FUNCTION_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define PD_FUNCTION_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define PD_FUNCTION_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define PD_FUNCTION_SIG __func__
	#else
		#define PD_FUNCTION_SIG "Unknown PD_FUNCTION_SIG"
	#endif

    #define PD_PROFILE_BEGIN_SESSION(name, file)    ::Dewpsi::Instrumentation::Get().BeginSession(name, file)
    #define PD_PROFILE_END_SESSION()                ::Dewpsi::Instrumentation::Get().EndSession()
    #define PD_PROFILE_SCOPE_LINE2(name, line)      ::Dewpsi::InstrumentationTimer timer##line(name)
    #define PD_PROFILE_SCOPE_LINE(name, line)       PD_PROFILE_SCOPE_LINE2(name, line)
    #define PD_PROFILE_SCOPE(name)                  PD_PROFILE_SCOPE_LINE(name, __LINE__)
    #define PD_PROFILE_FUNCTION()                   PD_PROFILE_SCOPE(PD_FUNCTION_SIG)
#else
    #define PD_PROFILE_BEGIN_SESSION(name, file)
    #define PD_PROFILE_END_SESSION(name, file)
    #define PD_PROFILE_SCOPE_LINE2(name, line)
    #define PD_PROFILE_SCOPE_LINE(name, line)
    #define PD_FUNCTION_SIG                     "Unknown PD_FUNCTION_SIG"
    #define PD_PROFILE_SCOPE(sig)
    #define PD_PROFILE_FUNCTION()
#endif

#endif /* DEWPSI_DEBUG_H */
