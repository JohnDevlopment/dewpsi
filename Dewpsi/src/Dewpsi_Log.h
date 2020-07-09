#ifndef DEWPSI_LOG_H
#define DEWPSI_LOG_H

/**
*   @file       Dewpsi_Log.h
*   @brief      A header for the Dewpsi engine. It contains
*               functions and macros for logging text based on
*               log-level.
*
*   @defgroup   logging Logging
*   @ingroup    core
*   @{
*/

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Dewpsi {
    /** Main logger interface.
    *   @ingroup logging
    */
    class Log {
    public:
        /// Initialize the logger.
        static void Init();
        
        /// Returns true if the logger is initialized.
        static bool IsInit()
        {
            return s_Initted;
        }
        
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
    
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        
        static bool s_Initted;
    };
    
    /** Just a class that holds an arbitrary integral value.
    *   Can be passed to std::cout or any other ostream as well
    *   as the logger.
    *   @ingroup logging
    */
    template<typename T>
    class LogHex {
    public:
        /// Constructs a LogHex with an integral value.
        LogHex(T val) : m_value(val)
        {  }
        
        /// Returns the value.
        T get() const { return m_value; }
        
    private:
        T m_value;
    };
    
    /** Constructs a @c LogHex with the given value.
    *   @param  val Any integer value.
    *   @return     A @c LogHex with @a value in it.
    */
    template<typename T>
    inline typename std::enable_if<std::is_arithmetic<T>::value, LogHex<T>>::type
    MakeLogHex(T val)
    {
        return LogHex<T>(val);
    }
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Dewpsi::LogHex<T>& obj)
{
    os << std::hex << obj.get() << std::dec;
}

/// Logs trace messages.
#define PD_CORE_TRACE(...)     ::Dewpsi::Log::GetCoreLogger()->trace(__VA_ARGS__)
/// Logs info messages.
#define PD_CORE_INFO(...)      ::Dewpsi::Log::GetCoreLogger()->info(__VA_ARGS__)
/// Prints a warning.
#define PD_CORE_WARN(...)      ::Dewpsi::Log::GetCoreLogger()->warn(__VA_ARGS__)
/// Prints an error.
#define PD_CORE_ERROR(...)     ::Dewpsi::Log::GetCoreLogger()->error(__VA_ARGS__)
/// Prints a @a fatal @a error.
#define PD_CORE_CRITICAL(...)  ::Dewpsi::Log::GetCoreLogger()->critical(__VA_ARGS__)

/// Logs trace messages.
#define PD_TRACE(...)          ::Dewpsi::Log::GetClientLogger()->trace(__VA_ARGS__)
/// Logs info messages.
#define PD_INFO(...)           ::Dewpsi::Log::GetClientLogger()->info(__VA_ARGS__)
/// Prints a warning.
#define PD_WARN(...)           ::Dewpsi::Log::GetClientLogger()->warn(__VA_ARGS__)
/// Prints an error.
#define PD_ERROR(...)          ::Dewpsi::Log::GetClientLogger()->error(__VA_ARGS__)
/// Prints a @a fatal @a error.
#define PD_CRITICAL(...)       ::Dewpsi::Log::GetClientLogger()->critical(__VA_ARGS__)

/// @}

#endif /* DEWPSI_LOG_H */
