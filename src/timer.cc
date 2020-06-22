#include "timer.h"
#include <chrono>
#include <thread>

namespace Dewpsi {

Timer::Timer() : m_start(), m_end(), m_elapsed()
{  }

Timer::~Timer()
{  }

void Timer::Start()
{
    m_start = std::chrono::steady_clock::now();
}

double Timer::Stop()
{
    m_end = std::chrono::steady_clock::now();
    m_elapsed = std::chrono::duration_cast<_Seconds_t>(m_end - m_start);
    
    return Get();
}

double Timer::Get() const
{
    return m_elapsed.count();
}

void Timer::Sleep(double dSeconds)
{
    std::chrono::duration<double, std::ratio<1,1>> dtn(dSeconds);
    std::this_thread::sleep_for(dtn);
}

void Timer::Sleep(uint32_t uiMs)
{
    std::chrono::duration<double, std::ratio<1,1000>> dtn(uiMs);
    std::this_thread::sleep_for(dtn);
}

}

