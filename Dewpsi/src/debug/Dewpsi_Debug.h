#ifndef DEWPSI_DEBUG_H
#define DEWPSI_DEBUG_H

/**
*   @file Dewpsi_Debug.h
*   @ref debug
*   @defgroup debug Debugging
*   Contains debug macros.
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Timer.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <cassert>

#ifndef PD_DEBUG
#define NDEBUG
#endif

#ifndef PD_ENABLE_DEBUG_BREAKS
#define PD_ENABLE_DEBUG_BREAKS 0
#endif /* PD_ENABLE_DEBUG_BREAKS */

/**
*   @def        PD_DEBUGBREAK
*   @brief      Sends an interruption signal to the application.
*               This causes GDB and othe debuggers to set a breakpoint.
*/
#if defined(PD_DEBUG) && PD_ENABLE_DEBUG_BREAKS
# if defined(PD_PLATFORM_LINUX) || defined(__linux)
#  define PD_DEBUGBREAK() std::raise(SIGINT);
# elif defined(PD_PLATFORM_WINDOWS) || defined(_WIN32)
#  define PD_DEBUGBREAK() __debugbreak();
# else
#  warning "PD_DEBUGBREAK is undefined because the operating system is not recognized."
#  define PD_DEBUGBREAK()
# endif
#else
# define PD_DEBUGBREAK(m)
#endif

#ifndef PD_PROFILE
    #define PD_PROFILE 0
#endif

namespace Dewpsi {
    /// @addtogroup debug
    /// @{

    using FloatMicroSeconds = std::chrono::duration<double, std::micro>;

    //////////////////////////////////////////////////////////
    // Instrumentation ////////////////////////////////////////
    //////////////////////////////////////////////////////////

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
    #define PD_PROFILE_END_SESSION()
    #define PD_PROFILE_SCOPE_LINE2(name, line)
    #define PD_PROFILE_SCOPE_LINE(name, line)
    #define PD_FUNCTION_SIG                     "Unknown PD_FUNCTION_SIG"
    #define PD_PROFILE_SCOPE(sig)
    #define PD_PROFILE_FUNCTION()
#endif

#endif /* DEWPSI_DEBUG_H */
