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
#include <cstring>

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

    /*template<class T, size_t N>
    class StaticDArray {
    public:
        typedef std::decay<T>::type Type;

        StaticDArray()
            : m_pElements(new Type[N]),
              m_szLen(N)
        {
            // ctor
        }

    private:
        Type * m_pElements;
        const size_t m_szLen;
    };*/

    namespace String {
        /** Returns the first occurrence of @a ch in @a str.
        *   @param str A string
        *   @param ch  A character (byte) to find in @a str
        *   @return    A pointer to the first occurrence of @a ch in @a str
        *              or @c NULL if @a ch cannot be found
        */
        PD_FORCE_INLINE const char* StringChar(const char* str, int ch)
        {
            return ::std::strchr(str, ch);
        }

        /** Returns the last occurrence of @a ch in @a str.
        *   @param str A string
        *   @param ch  A character (byte) to find in @a str
        *   @return    A pointer to the last occurrence of @a ch in @a str
        *              or @c NULL if @a ch cannot be found
        */
        PD_FORCE_INLINE const char* StringRevChar(const char* str, int ch)
        {
            return ::std::strrchr(str, ch);
        }

        /** Returns the length of the string @a str.
        *
        *   @param  str A string, must not be @c NULL
        *   @return     The length of @a str in characters or PD_INVALID if @a str is @c NULL
        *   @ingroup strings
        */
        PD_FORCE_INLINE size_t Length(const char* str)
        {
            return ::std::strlen(str);
        }

        /** Fills a block of memory with the constant byte @a byte.
        *   This function sets the first @a len bytes in the memory
        *   area pointed to by @a dst with the constant byte @a byte.
        *
        *   @param[out] dst     The destination memory area
        *   @param[in]  byte    The value to set each byte to
        *   @param[in]  len     Length of the memory area to fill
        *   @return             @a Dst is returned
        */
        PD_CALL void MemSet(void* dst, int byte, size_t len);

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
        PD_CALL size_t Copy(char* dst, const char* src, size_t maxlen = -1, PDenum mode = PD_NULL);

        /** Concatenate two strings.
        *   Appends the @a src string at the @a dst string, overwriting the terminating null character '\0'
        *   at the end of @a dst, and adds a terminating null byte.
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
        *   @return         The total length of @a dst after concatenating the two strings
        *   @ingroup        strings
        */
        PD_CALL size_t Cat(char* dst, const char* src, size_t maxlen = -1);

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
        *
        *   @warning    After this call, the pointer is no longer valid and should not be used.
        *   @ingroup    strings
        */
        PD_INLINE void Delete(char* ptr)
        {
            delete[] ptr;
        }
    }
}

#endif /* DEWPSI_STRING_H */
