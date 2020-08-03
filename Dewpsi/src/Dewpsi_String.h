#ifndef DEWPSI_STRING_H
#define DEWPSI_STRING_H

/**
*   @defgroup   strings Strings
*   StaticString class and misc. string functions.
*   @ingroup    core
*/

/**
*   @file       Dewpsi_String.h
*   @brief      A header for the Dewpsi engine.
*   Contains string classes and functions.
*
*   @ingroup    strings
*/

#include <Dewpsi_Core.h>
#include <bits/Dewpsi_Macros.h>


namespace Dewpsi {
    /** A static string class.
    *   @ingroup strings
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
    
    namespace String {
        /** Returns the length of the string @a str.
        *   
        *   @param  str A string, must not be @c NULL
        *   @return     The length of @a str in characters or PD_INVALID if @a str is @c NULL
        *   @ingroup strings
        */
        PD_CALL size_t Length(const char* str);
        
        /** Copy a string.
        *   Copies the string pointed to by @a src to the destination buffer pointed to by @a dst,
        *   including the null terminator '\0' at the very end. No more than @a maxlen characters
        *   (including the null terminator) will be copied.
        *
        *   If any parameter except @a mode is @c NULL, Copy() returns @c PD_INVALID and nothing
        *   else is done. In that case, you can call GetError() to retrieve the specific error message.
        *
        *   @warning        The destination string @a dst must be large enough to hold
        *                   @a maxlen characters and must not overlap with @a src.
        *
        *   @param  dst     The destination buffer
        *   @param  src     The string whose characters are copied to @a dst
        *   @param  maxlen  The maximum number of characters to copy
        *   @param  mode    The only recognized value is PD_ALLOC
        *   @return         The number of characters copied or PD_INVALID on failure
        *   @ingroup strings
        */
        PD_CALL size_t Copy(char* dst, const char* src, size_t maxlen, PDenum mode = PD_NULL);
        
        /** Allocates a character array.
        *   Allocates an array of @a cnt characters and returns a pointer to it.
        *   The memory is set to the value @a ch, which defaults to zero. If it
        *   fails to allocate memory (because there is no more of it), a @c NULL
        *   pointer is returned and an error message is generated.
        *   
        *   @param  cnt String character length
        *   @param  ch  Fill character
        *   @return     A pointer to the allocated array or @c NULL if it ran out of memory
        
        *   @ingroup strings
        */
        PD_CALL char* New(const size_t cnt, char ch = '\0');
        
        /** Deletes the string.
        *   Frees the memory holding the string pointed to by @a src.
        *   
        *   @param  ptr The pointer to free
        *   @warning    After this call, the pointer is no longer valid.
        *   @ingroup strings
        */
        PD_INLINE void Delete(char* ptr)
        {
            delete[] ptr;
        }
    }
}

#endif /* DEWPSI_STRING_H */
