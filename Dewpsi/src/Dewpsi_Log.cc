#include "Dewpsi_Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <cstdarg>
#include <cstdio>

static std::unique_ptr<char[]> g_pError(new char[500]);

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

std::shared_ptr<spdlog::logger> Log::NewFileLogger(const PDstring& name, const PDstring& file)
{
    return std::shared_ptr<spdlog::logger>(spdlog::basic_logger_mt(name, file, true));
}

void SetError(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::vsprintf(g_pError.get(), fmt, args);
    va_end(args);
}

const char* GetError()
{
    return g_pError.get();
}

}
