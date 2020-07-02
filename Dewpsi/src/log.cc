#include "log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Dewpsi {

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

bool Log::s_Initted = false;

void Log::Init()
{
    spdlog::set_pattern("%^ [%T] %n: %v%$");
    
    s_CoreLogger = spdlog::stdout_color_mt("DEWPSI");
    s_CoreLogger->set_level(spdlog::level::trace);
    
    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
    
    s_Initted = true;
}

}

