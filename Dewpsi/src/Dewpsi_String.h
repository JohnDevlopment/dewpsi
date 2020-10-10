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
        typedef const char* iterator; ///< Iterator type

        /// Statically links @a str into the code.
        template<size_t N>
        constexpr StaticString(const char (&str)[N]) : m_String(str), m_Length(N)
        {  }

        /// Returns the a string.
        constexpr const char* get() const
        { return m_String; }

        /// Returns the length of the string.
        constexpr size_t size() const
        { return m_Length; }

        /// Returns an iterator to the beginning of the string.
        constexpr iterator begin() const {return m_String;}

        /// Returns an iterator to the end of the string.
        constexpr iterator end() const {return m_String + m_Length;}

    private:
        const char* m_String;
        const size_t m_Length;
    };

    namespace String {
        /** Returns the first occurrence of @a ch in @a str.
        *   @param str A string
        *   @param ch  A character (byte) to find in @a str
        *   @return    A pointer to the first occurrence of @a ch in @a str
        *              or @c NULL if @a ch cannot be found
        *   @ingroup   strings
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
        *   @ingroup   strings
        */
        PD_FORCE_INLINE const char* StringRevChar(const char* str, int ch)
        {
            return ::std::strrchr(str, ch);
        }

        /** Convert a lowercase character to an uppercase character.
        *   @param  ch  Character to be converted casted as an integer
        *   @return     The uppercase equivalent of @a ch, if there is one, or @a ch otherwise
        *   @ingroup    strings
        */
        PD_FORCE_INLINE char ToUpper(int ch)
        {
            constexpr char diff = 'a' - 'A';
            return ((char) ch >= 'a') && ((char) ch <= 'z') ? (char) ch - diff : (char) ch;
        }

        /** Convert a uppercase character to an lowercase character.
        *   @param  ch  Character to be converted casted as an integer
        *   @return     The uppercase equivalent of @a ch, if there is one, or @a ch otherwise
        *   @ingroup    strings
        */
        PD_FORCE_INLINE char ToLower(int ch)
        {
            constexpr char diff = 'a' - 'A';
            return ((char) ch >= 'A') && ((char) ch <= 'Z') ? (char) ch + diff : (char) ch;
        }

        /** Returns the length of the string @a str.
        *
        *   @param  str A string, must not be @c NULL
        *   @return     The length of @a str in characters or PD_INVALID if @a str is @c NULL
        *   @ingroup    strings
        */
        PD_FORCE_INLINE size_t Length(const char* str)
        {
            return ::std::strlen(str);
        }

        /** Parses the C-string @a str, interpreting it as an integer.
        *   @param  str A C-string with the characters to be interpreted as digits. Depending
        *               on the first two characters, the string can be interpreted in one of three
        *               ways: 1. if the first two characters are "0x", interpret as a hexadecimal
        *               string; 2. if the first character is a zero, and the second isn't 'x',
        *               interpret as an octal number; 3. if all other tests fail, interpret as a
        *               decimal number.
        *   @return     A %c long %c int containing the converted number. If the result is zero,
        *               and the string contained non-zero digits, then that means the string
        *               exceeded the integer range. Call GetError() to retrieve the error message.
        *   @ingroup    strings
        */
        PD_CALL long int StringToLong(const char* str);

        /** Parses the C-string @a str, interpreting it as an integer.
        *   @param  str A C-string with the characters to be interpreted as digits. Depending
        *               on the first two characters, the string can be interpreted in one of three
        *               ways: 1. if the first two characters are "0x", interpret as a hexadecimal
        *               string; 2. if the first character is a zero, and the second isn't 'x',
        *               interpret as an octal number; 3. if all other tests fail, interpret as a
        *               decimal number.
        *   @return     An %c int containing the converted number. If the result is zero,
        *               and the string contained non-zero digits, then that means the string
        *               exceeded the integer range. Call GetError() to retrieve the error message.
        *   @ingroup    strings
        */
        PD_FORCE_INLINE int StringToInt(const char* str)
        {
            return static_cast<int>(StringToLong(str));
        }

        /** Fills a block of memory with the constant byte @a byte.
        *   This function sets the first @a len bytes in the memory
        *   area pointed to by @a dst with the constant byte @a byte.
        *
        *   @param[out] dst     The destination memory area
        *   @param[in]  byte    The value to set each byte to
        *   @param[in]  len     Length of the memory area to fill
        *   @return             @a Dst is returned
        *   @ingroup            strings
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
        *   @ingroup        strings
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
        *   @ingroup    strings
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
