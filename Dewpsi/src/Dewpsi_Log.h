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

#include <Dewpsi_Core.h>
#include <Dewpsi_Memory.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Dewpsi {
    /** Main logger interface.
    *   @ingroup logging
    */
    class Log {
    public:
        /// Initialize the logger.
        static void Init();

        /// Returns true if the logger is initialized.
        static bool IsInit() {return s_Initted;}

        /// Returns a pointer to the core logger.
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger;}

        /// Returns a pointer to the client logger.
        static std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger;}

        /** Creates a new file logger.
        *   @param  file    A string containing the path to a file to write to
        *   @return         A shared pointer to the file logger
        */
        static std::shared_ptr<spdlog::logger> NewFileLogger(const PDstring& name, const PDstring& file);

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
    *   @ingroup    logging
    */
    template<typename T>
    inline typename std::enable_if<std::is_arithmetic<T>::value, LogHex<T>>::type
    MakeLogHex(T val)
    {
        return LogHex<T>(val);
    }

    /** Sets an error message.
    *   @param  fmt A format string in the syntax used by printf
    *   @param  ... A list of arguments that correspond to the fields in @a fmt
    *   @warning    The error string max size is 500 characters.
    *   @ingroup    logging
    */
    PD_CALL void SetError(const char* fmt, ...);

    /** Returns the error message last set by SetError().
    *   @return The error message last set by SetError()
    *   @ingroup    logging
    */
    PD_CALL const char* GetError();
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Dewpsi::LogHex<T>& obj)
{
    os << std::hex << "0x" << obj.get() << std::dec;
}

/** Sets an error string for an invalid function parameter.
*   @param  param   The name of a parameter as a string
*   @par    Example
*   @code
    int add(int param) {
        if (param < 0)
            PD_BADPARAM("param");
        param += 5;
        return param;
    }
*   @endcode
*/
#define PD_BADPARAM(param)      ::Dewpsi::SetError("Bad parameter '%s'", param)

/// Sets an error string indicating a lack of memory.
#define PD_NOMEMORY()           ::Dewpsi::SetError("Ran out of memory")

#ifdef PD_DEBUG
    /// Prints the name of the function.
    #define PD_CORE_PRINTFUNC() ::Dewpsi::Log::GetCoreLogger()->trace("Function name: {0}", __FUNCTION__)
    /// Prints the name of the function.
    #define PD_PRINTFUNC()      ::Dewpsi::Log::GetClientLogger()->trace("Function name: {0}", __FUNCTION__)
    #define PD_DTRACE(fmt, ...) ::Dewpsi::Log::GetClientLogger()->trace("{}: " fmt, __FUNCTION__, __VA_ARGS__)
#else
    #define PD_CORE_PRINTFUNC()
    #define PD_PRINTFUNC()
    #define PD_DTRACE(fmt, ...)
#endif

/// Logs trace messages.
#define PD_CORE_TRACE(...)      ::Dewpsi::Log::GetCoreLogger()->trace(__VA_ARGS__)
/// Logs info messages.
#define PD_CORE_INFO(...)       ::Dewpsi::Log::GetCoreLogger()->info(__VA_ARGS__)
/// Prints a warning.
#define PD_CORE_WARN(...)       ::Dewpsi::Log::GetCoreLogger()->warn(__VA_ARGS__)
/// Prints an error.
#define PD_CORE_ERROR(...)      ::Dewpsi::Log::GetCoreLogger()->error(__VA_ARGS__)
/// Prints a @a fatal @a error.
#define PD_CORE_CRITICAL(...)   ::Dewpsi::Log::GetCoreLogger()->critical(__VA_ARGS__)

/// Logs trace messages.
#define PD_TRACE(...)           ::Dewpsi::Log::GetClientLogger()->trace(__VA_ARGS__)
/// Logs info messages.
#define PD_INFO(...)            ::Dewpsi::Log::GetClientLogger()->info(__VA_ARGS__)
/// Prints a warning.
#define PD_WARN(...)            ::Dewpsi::Log::GetClientLogger()->warn(__VA_ARGS__)
/// Prints an error.
#define PD_ERROR(...)           ::Dewpsi::Log::GetClientLogger()->error(__VA_ARGS__)
/// Prints a @a fatal @a error.
#define PD_CRITICAL(...)        ::Dewpsi::Log::GetClientLogger()->critical(__VA_ARGS__)

/// @}

#endif /* DEWPSI_LOG_H */
