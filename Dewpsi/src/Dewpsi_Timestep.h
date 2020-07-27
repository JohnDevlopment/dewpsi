#ifndef DEWPSI_TIMESTEP_H
#define DEWPSI_TIMESTEP_H

/**
*   @file       Dewpsi_Timestep.h
*   @brief      A header for the Dewpsi engine.
*   @ingroup    core
*/

namespace Dewpsi {
    /** A timestep.
    *   @ingroup core
    */
    class Timestep {
    public:
        /** Constructs a Timestep.
        *   @param time Initial value of the Timestep
        */
        Timestep(float time = 0.0f) : m_fTime(time)
        {  }
        
        /// Destructor
        ~Timestep()
        {  }
        
        /// Returns the amount of seconds, calls GetSeconds().
        operator float() const
        {
            return GetSeconds();
        }
        
        /// Set timestep based on milliseconds
        template<typename T>
        void SetMilliseconds(T ms)
        {
            m_fTime = (float) ms / 1000.0f;
        }
        
        /// Returns the amount of seconds.
        float GetSeconds() const
        {
            return m_fTime;
        }
        
        /// Returns the amount of milliseconds.
        float GetMilliseconds() const
        {
            return m_fTime * 1000.0f;
        }
        
    private:
        float m_fTime;
    };
}

#endif /* DEWPSI_TIMESTEP_H */
