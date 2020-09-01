#ifndef TIMER_H
#define TIMER_H

/** @file Dewpsi_Timer.h
*   @brief @doxfb
*   Timers
*   @defgroup timers Timers
*   @ingroup core
*/

#include <Dewpsi_Log.h>
#include <thread>
#include <chrono>
#include <cstdint>

namespace Dewpsi {
    /** Generic timer.
    *   @ingroup timers
    */
    class Timer {
    public:
        Timer() : m_Start(), m_End(), m_Elapsed(), m_Stopped(true) {}
        ~Timer() {}

        /// Starts the timer.
        void Start()
        {
            if (m_Stopped)
            {
                m_Start = std::chrono::steady_clock::now();
                m_Stopped = false;
            }
        }

        /// Stops the timer and returns the amount of milliseconds elapsed.
        double Stop()
        {
            m_End = std::chrono::steady_clock::now();
            m_Elapsed = std::chrono::duration_cast<_Milliseconds_t>(m_End - m_Start);

            return m_Elapsed.count();
        }

        /// Returns the amount of milliseconds elapsed.
        double Get() const {return m_Elapsed.count();}

        /** Pauses execution of the current thread for a given amount of time.
        *   @param seconds The amount of seconds to sleep
        */
        static void Sleep(double seconds)
        {
            std::chrono::duration<double, std::ratio<1,1>> dtn(seconds);
            std::this_thread::sleep_for(dtn);
        }

        /** Pauses execution of the current thread for a given amount of time.
        *   @param ms The amount of milliseconds to sleep
        */
        static void Sleep(uint32_t ms)
        {
            std::chrono::duration<double, std::ratio<1,1000>> dtn(ms);
            std::this_thread::sleep_for(dtn);
        }

    private:
        typedef std::chrono::steady_clock::time_point _TimePoint;
        typedef std::chrono::duration<double,std::ratio<1,1>> _Milliseconds_t;

        _TimePoint m_Start, m_End;
        _Milliseconds_t m_Elapsed;
        bool m_Stopped;
    };

    /** A timer that starts on construction and ends on destruction.
    *   @ingroup timers
    */
    class ScopeTimer {
    public:
        ScopeTimer() : m_Start(), m_End(), m_Elapsed(), m_Stopped(true) {Start();}

        ~ScopeTimer() {Stop();}

        /// Starts the timer.
        void Start()
        {
            m_Start = std::chrono::steady_clock::now();
            m_Stopped = false;
        }

        /// Stops the timer and records the time elapsed.
        void Stop()
        {
            if (! m_Stopped)
            {
                using std::chrono::microseconds;

                m_End = std::chrono::steady_clock::now();
                PDllong lStart = std::chrono::time_point_cast<_Microseconds_t>(m_Start).time_since_epoch().count();
                PDllong lEnd = std::chrono::time_point_cast<_Microseconds_t>(m_End).time_since_epoch().count();

                m_Elapsed = m_End - m_Start;
                m_Milliseconds = (lEnd - lStart) * 0.001f; // to milliseconds
                m_Stopped = true;
            }
        }

        /// Get the time elapsed in microseconds.
        float Get() const {return m_Elapsed.count();}

        /// Get the time elapsed in milliseconds.
        float GetMilliseconds() const {return m_Milliseconds;}

    protected:
        typedef std::chrono::steady_clock::time_point _TimePoint;
        typedef std::chrono::duration<double,std::ratio<1,1000000>> _Microseconds_t;

        _TimePoint m_Start, m_End;
        _Microseconds_t m_Elapsed;
        double m_Milliseconds;
        bool m_Stopped;
    };
}

#endif /* TIMER_H */
