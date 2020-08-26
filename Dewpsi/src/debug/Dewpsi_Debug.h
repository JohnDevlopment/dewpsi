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

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>

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
}

#if PD_PROFILE > 0
    #if (defined(__GNUC__) || defined(__GNUG__))
        #define PD_FUNCTION_SIG         __PRETTY_FUNCTION__
    #else
        #define PD_FUNCTION_SIG         "Unknown PD_FUNCTION_SIG"
    #endif

    #define PD_PROFILE_SCOPE(sig)
    #define PD_PROFILE_FUNCTION()   PD_PROFILE_SCOPE(PD_FUNCTION_SIG)
#else
    #define PD_FUNCTION_SIG         "Unknown PD_FUNCTION_SIG"
    #define PD_PROFILE_SCOPE(sig)
    #define PD_PROFILE_FUNCTION()
#endif

#endif /* DEWPSI_DEBUG_H */
