#ifndef LOG_H
#define LOG_H

/**
*   @file       log.h
*   @brief      A header for the Dewpsi engine. It contains
*               functions and macros for logging text based on
*               log-level.
*
*   @defgroup   logging
*   @ingroup    core
*/

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Dewpsi {
    /** Main logger interface.
    *   @ingroup logging
    */
    class Log {
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        
    public:
        /// Initialize the logger.
        static void Init();
        
        /// Returns a pointer to the core logger.
        static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        }
        
        /// Returns a pointer to the client logger.
        static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        }
    };
}

/// Logs trace messages.
#define PD_CORE_TRACE(...)  ::Dewpsi::Log::GetCoreLogger()->trace(__VA_ARGS__)
/// Logs info messages.
#define PD_CORE_INFO(...)   ::Dewpsi::Log::GetCoreLogger()->info(__VA_ARGS__)
/// Prints a warning.
#define PD_CORE_WARN(...)   ::Dewpsi::Log::GetCoreLogger()->warn(__VA_ARGS__)
/// Prints an error.
#define PD_CORE_ERROR(...)  ::Dewpsi::Log::GetCoreLogger()->error(__VA_ARGS__)
/// Prints a @i{fatal error}.
#define PD_CORE_FATAL(...)  ::Dewpsi::Log::GetCoreLogger()->fatal(__VA_ARGS__)

/// Logs trace messages.
#define PD_TRACE(...)   ::Dewpsi::Log::GetClientLogger()->trace(__VA_ARGS__)
/// Logs info messages.
#define PD_INFO(...)    ::Dewpsi::Log::GetClientLogger()->info(__VA_ARGS__)
/// Prints a warning.
#define PD_WARN(...)    ::Dewpsi::Log::GetClientLogger()->warn(__VA_ARGS__)
/// Prints an error.
#define PD_ERROR(...)   ::Dewpsi::Log::GetClientLogger()->error(__VA_ARGS__)
/// Prints a @i{fatal error}.
#define PD_FATAL(...)   ::Dewpsi::Log::GetClientLogger()->fatal(__VA_ARGS__)

#endif /* LOG_H */

