#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstdint>

namespace Dewpsi {
    class Timer {
        typedef std::chrono::steady_clock::time_point _TimePoint;
        typedef std::chrono::duration<double,std::ratio<1,1>> _Seconds_t;
        
        _TimePoint m_start, m_end;
        _Seconds_t m_elapsed;
        
    public:
        Timer();
        ~Timer();
        
        void Start();
        double Stop();
        double Get() const;
        
        // pauses execution of the current thread for a given amount of time
        static void Sleep(double dSeconds);
        static void Sleep(uint32_t uiMs);
    };
}

#endif /* TIMER_H */

