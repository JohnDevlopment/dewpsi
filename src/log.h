#ifndef LOG_H
#define LOG_H

#include <memory>
#include <spdlog/spdlog.h>

namespace Dewpsi {
    class Log {
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        
    public:
        static void Init();
        
        static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        }
        
        static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        }
    };
}

// logging macros
#define PD_CORE_TRACE(...) ::Dewpsi::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PD_CORE_INFO(...) ::Dewpsi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PD_CORE_WARN(...) ::Dewpsi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PD_CORE_ERROR(...) ::Dewpsi::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PD_CORE_FATAL(...) ::Dewpsi::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define PD_TRACE(...) ::Dewpsi::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PD_INFO(...) ::Dewpsi::Log::GetClientLogger()->info(__VA_ARGS__)
#define PD_WARN(...) ::Dewpsi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PD_ERROR(...) ::Dewpsi::Log::GetClientLogger()->error(__VA_ARGS__)
#define PD_FATAL(...) ::Dewpsi::Log::GetClientLogger()->fatal(__VA_ARGS__)

#endif /* LOG_H */

