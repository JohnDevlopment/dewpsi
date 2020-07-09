#ifndef DEWPSI_STRING_H
#define DEWPSI_STRING_H

/**
*   @file       Dewpsi_String.h
*   @brief      A header for the Dewpsi engine.
*   Contains string classes and functions.
*
*   @ingroup    core
*/

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /** A static string class.
    *   @ingroup core
    */
    class StaticString {
    public:
        /// Statically links @a str into the code.
        template<size_t N>
        constexpr StaticString(const char (&str)[N]) : m_string(str), m_length(N)
        {  }
        
        /// Returns the a string.
        constexpr const char* get() const
        { return m_string; }
        
        /// Returns the length of the string.
        constexpr size_t size() const
        { return m_length; }
        
    private:
        const char* m_string;
        const size_t m_length;
    };
}

#endif /* DEWPSI_STRING_H */
