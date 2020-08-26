#ifndef CORE_H
#define CORE_H

/**
*   @file       Dewpsi_Core.h
*   @brief      @doxfb
*   Contains preprocessor macros.
*
*   @defgroup   core Core
*   Provisions of the core functionality of Dewpsi.
*   For function attributes, see the following.
*
*   Macro           | Description
*	----------------|------------
*	PD_INLINE       | Suggest that a function be inlined
*	PD_FORCE_INLINE | force a function to be inlined
*	PD_COLD         | Declare a function to be cold
*	PD_HOT          | Declare a function to be hot
*	PD_CALL         | Prefix to an export library call
*
*	For utility macros, see:
*   - PD_ABORT()         - abort execution of a program
*	- PD_OFFSETOF()      - get the offset to a member of a type
*	- PD_ARRAYSIZE()     - to measure the length of an array
*	- PD_ADDRESSOF()     - to get the address of an object
*	- PD_BIND_EVENT_FN() - bind an event function to a class
*	- PD_LOWORD()        - get the low word of a dword
*	- PD_HIWORD()        - get the high word of a dword
*	- PD_CREATEDWORD()   - create a dword from two words
*	- PD_LONYBBLE()      - get the low nybble of a byte
*	- PD_HINYBBLE()      - get the high nybble of a byte
*	- PD_CREATEBYTE()    - create a byte from two nybbles
*
*   For other kinds of macros, see:
*	- @ref PD_CONSTEXPR
*	- @ref PD_APIENTRY
*	- PD_DEBUGBREAK()
*	- PD_ASSERT()
*	- PD_CORE_ASSERT()
*
*   @defgroup platforms Platform-Specific
*   Headers, functions, types, and other declarations specific to each platform.
*
*   @addtogroup core
*   @{
*/

#include <Dewpsi_cxx11.h>
#include <Dewpsi_Types.h>

#include <iostream>
#include <new>
#include <utility>
#include <type_traits>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>
//#define _IMPORT_MY_GETOPT
#include <my_getopt.h>

#include <csignal>
#include <cstdint>

/// Convert an lvalue to an rvalue.
#define PD_MOVE(e)  std::move(e)

/** @defgroup func_attrs Function Attributes
*	In order to use attributes, use the PD_ATTR macro. It accepts a list
*	of attributes that are to be applied to a function or other declaration.
*   @ingroup core
*	@{
*/

#ifndef PD_ATTR
    #if defined(__GNUC__)
        /** Define a list of attributes for a function or variable.
        *   @param ... One or more attributes to apply
        *	@note For a list of attributes, see your compiler's documentation.
        */
        #define PD_ATTR(...) __attribute__((__VA_ARGS__))
    #elif defined(_MSC_VER) || defined(__BORLANDC__)
        #define PD_ATTR(...) __declspec(__VA_ARGS__)
    #else
        #error "This compiler is not supported"
    #endif
#endif

/**
*   @def    PD_COLD
*   @brief  Mark a function as cold.
*   @code
    int add(int a, int b) PD_ATTR(PD_COLD);
*   @endcode
*/
#ifndef PD_COLD
    #define PD_COLD cold
#endif

/**
*   @def    PD_HOT
*   @brief  Mark a function as hot.
*   @code
    int add(int a, int b) PD_ATTR(PD_HOT);
*   @endcode
*/
#ifndef PD_HOT
    #define PD_HOT hot
#endif

/**
*   @def    PD_INLINE
*   @brief  Marks a function as inline.
*   @code
    PD_INLINE int add(int a, int b)
    {
        return a + b;
    }
*   @endcode
*/
#ifndef PD_INLINE
    #if defined(__GNUC__)
        #define PD_INLINE inline
        #ifndef __inline__
            #define __inline__ inline
        #endif
    #elif defined(_MSC_VER) || defined(__BORLANDC__) || \
          defined(__DMC__) || defined(__SC__) || \
          defined(__WATCOMC__) || defined(__LCC__) || \
          defined(__DECC) || defined(__CC_ARM)
        #define PD_INLINE __inline
        #ifndef __inline__
            #define __inline__ __inline
        #endif
    #else
        #define PD_INLINE
    #endif
#endif

/**
*   @def    PD_FORCE_INLINE
*   @brief  Forces a function to be inline.
*   @code
    PD_FORCE_INLINE int add(int a, int b)
    {
        return a + b;
    }
*   @endcode
*/
#ifndef PD_FORCE_INLINE
    #if ((defined(__GNUC__) && (__GNUC__ >= 4))) || defined(__clang__)
        #define PD_FORCE_INLINE __attribute__((always_inline)) static __inline__
    #elif defined(_MSC_VER)
        #define PD_FORCE_INLINE __forceinline
    #else
        #define PD_FORCE_INLINE static PD_INLINE
    #endif
#endif

/**
*   @def        PD_CALL
*   @brief      Prefix to the declaration of a non-static non-member function.
*	It expands to different things depending on the platform and how the library
*	is being built. (Currently it does not mean anything.)
*/
#if (defined(PD_PLATFORM_LINUX) || defined(__linux__) || defined(__gnu_linux__)) \
         && (defined(__GNU__) || defined(__GNUG__))
    #define PD_CALL extern
#elif defined(PD_PLATFORM_WINDOWS) || defined(_WIN32)
    #if defined(PD_EXPORT_DLL)
        #define PD_CALL     __declspec(dllexport) extern
    #elif defined(PD_IMPORT_DLL)
        #define PD_CALL     __declspec(dllimport) extern
    #else
        #define PD_CALL
        #error "Define either PD_EXPORT_DLL or PD_IMPORT_DLL"
    #endif
#else
    #define PD_CALL
    #error "This platform is not supported!"
#endif

/**
*   @def    PD_APIENTRY
*   @brief  Used to declare a function pointer an exported symbol.
*   @code
    int (PD_APIENTRY * func)(int);
*   @endcode
*/
#define PD_APIENTRY

/*#ifdef __GNUC__
    #define PD_APIENTRY extern
#elif defined(_MSC_VER)
    #if defined(PD_EXPORT_DLL)
        #define PD_APIENTRY __declspec(dllexport)
    #elif defined(PD_IMPORT_DLL)
        #define PD_APIENTRY __declspec(dllimport)
    #else
        #define PD_APIENTRY extern
    #endif
#endif*/

/// @}
// end of group core -> "Function Attributes"

/** Get the byte offset of a member of a structure or class.
*   @param  type    A type that is a structure or class
*   @param  member  A member of @a type
*   @return         The byte offset of @a member inside of @a type
*
*   @par Example
*   @snippet core.cpp Offsetof
*/
#define PD_OFFSETOF(type, member)   offsetof(type, member)

/** Get the size of a static array.
*   @param  a   An array with static storage
*   @return     The number of elements in the array
*/
#define PD_ARRAYSIZE(a)             static_cast<int>(sizeof(a) / sizeof(* (a)))

/** Get the address of @a o.
*   Simply calls std::addressof(). Refer to the <a href="http://www.cplusplus.com/reference/memory/addressof/">documentation</a>
*   for that function.
*/
#define PD_ADDRESSOF(o)             ::std::addressof(o)

/** Prefix to the declaration of any function or variable.
*   Indicates that a function or variable is @c constexpr.
*/
#define PD_CONSTEXPR constexpr

/**
*   @def        PD_DEBUGBREAK
*   @brief      Sends an interruption signal to the application.
*               This causes GDB and othe debuggers to set a breakpoint.
*/
// debug breaking
#ifdef PD_DEBUG
    #if defined(PD_PLATFORM_LINUX) || defined(__linux)
        #define PD_DEBUGBREAK() std::raise(SIGINT);
    #elif defined(PD_PLATFORM_WINDOWS) || defined(_WIN32)
        #define PD_DEBUGBREAK() __debugbreak();
    #else
        #warning PD_DEBUGBREAK is undefined because the operating system is not recognized.
        #define PD_DEBUGBREAK()
    #endif
#else
    #define PD_DEBUGBREAK(m)
#endif

/// Aborts execution of the program.
#define PD_ABORT()  std::abort()

/**
*   @def        PD_ASSERT
*   @brief      Summarily stops execution of the application if @a x evaluates as @c false.
*   @param  x   The expression to evaluate.
*   @param  ... Forwarded to PD_ERROR().
*   @note       Only compiles if @a PD_ENABLE_ASSERTS is defined.
*
*   @def        PD_CORE_ASSERT
*   @brief      Summarily stops execution of the application if @a x evaluates as @c false.
*   @param  x   The expression to evaluate.
*   @param  ... Forwarded to PD_CORE_ERROR().
*   @note       Only compiles if @a PD_ENABLE_ASSERTS is defined.
*/
#ifdef PD_ENABLE_ASSERTS
    #define PD_ASSERT(x, ...)       if (! (x)) { \
                                        ::Dewpsi::Log::GetClientLogger()->error("Assertion '" #x "' failed"); \
                                        ::Dewpsi::Log::GetClientLogger()->error(__VA_ARGS__); \
                                        PD_ABORT(); \
                                    }
    #define PD_CORE_ASSERT(x, ...)  if (! (x)) { \
                                        ::Dewpsi::Log::GetCoreLogger()->error("Assertion '" #x "' failed"); \
                                        ::Dewpsi::Log::GetCoreLogger()->error(__VA_ARGS__); \
                                        PD_ABORT(); \
                                    }
#else
    #define PD_ASSERT(x, ...)
    #define PD_CORE_ASSERT(x, ...)
#endif

/// Creates a value where only bit @a x is set.
#define BIT(x)                  (1 << x)

/** Used by classes to bind an event to the class.
*   Binds the function @a fn to the class that this is called in.
*   @code
    template<class Tp, class EventType>
    bool EventCallback(Tp* this, EventType& e);
*   @endcode
*   As shown here, the function must return @c void and accept two
*	parameters, the first being the class' @a this pointer, and the other
*	a reference to any type that derives from Event.
*/
#define PD_BIND_EVENT_FN(fn)    std::bind(&fn, this, std::placeholders::_1)

/// Retrieves the low word of a value.
#define PD_LOWORD(x)            ((x) & 0xffff)

/// Retrieves the high word of a value.
#define PD_HIWORD(x)            ((x) >> 16)

/** Defines a 32-bit integer from two 16-bit values.
*   @param  lo  The low word
*   @param  hi  The high word.
*   @return     A 32 bit value constructed from two words
*
*   @par Example
*   An example of how PD_CREATEDWORD() can be used:
*   @code{.cpp}
    PDint32 iVal = PD_CREATEDWORD(0x0f, 0x05); // results in 0x010f
    std::cout << std::hex << "0x" << iVal << std::endl;
*   @endcode
*/
#define PD_CREATEDWORD(lo, hi)  (((PDuint16)(hi) << 16) | (PDuint16)(lo))

/// Retrieves the low nybble of an 8-bit value.
#define PD_LONYBBLE(x)          ((PDint8)(x) & 0x0f)

/// Retrieves the high nybble of an 8-bit value.
#define PD_HINYBBLE(x)          ((PDuint8)(x) >> 4)

/** Defines an 8 bit value from two integers.
*   The value is created by combining two integers, one for the low nybble
*   (a nybble is 4 bits), and one for the high nybble.
*   @param  lo  The low nybble
*   @param  hi  The high nybble
*   @return     An 8-bit integer with the two values combined
*
*   @par Example
*   To show how this macro can be used, let's create an 8 bit integer from two
*   smaller values.
*
*   @code{.cpp}
    PDint8 uiVal = PD_CREATEBYTE(0x0f, 0x01); // results in 0x1f
    std::cout << std::hex << "0x" << uiVal << std::endl;
*   @endcode
*/
#define PD_CREATEBYTE(lo, hi)   (((PDuint8)(hi) << 4) | (((PDuint8)(lo)) & 0x0f))

/// Dewpsi Namespace: contains all relevent Dewpsi functions, classes, and other types.
namespace Dewpsi {
    /// @addtogroup core
    /// @{

    /// Returns 1 or 0 depending on whether @a val is non-zero.
    template<typename T>
    inline typename std::enable_if<std::is_arithmetic<T>::value, int>::type
    IsNonzero(T val)
    {
        return (val) ? 1 : 0;
    }

    /** Exchanges the values of @a a and @a b.
    *   The parameters are of course @a a and @a b, whose values are swapped.
    *   The type @c T must be move constructible and move assignable.
    */
    template<typename T>
    void Swap (T& a, T& b)
    noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value)
    {
        T temp(std::move(a));
        a = std::move(b);
        b = std::move(temp);
    }

    /** Parses commandline arguments.
    *   The @a argc and @a argv arguments are the same as the ones passed to main().
    *	Each ARGV-element is processed with each successive call to this function.
    *   An element that starts with a '-' and is not exactly '-' or "--" is
    *	an option. The characters of this element are option characters (not
    *	counting the initial '-').
    *
    *   The external variable @a optind is the index to the next element in
    *	@a argv. The system initializes it to 1. And when an option character is
    *	found, optind is updated to point to the next ARGV-element.
    *
    *   The @a optstring parameter contains the list of option characters
    *   that are recognized. If a character is followed by a colon, it requires
    *   an argument; if there are two colons, it is optional. A pointer to the
    *   following text in the same ARGV-element or the text in the following
    *   element is placed in the external variable @a optarg.

    *   @par Ordering
    *   By default, this function permutes the contents of @a argv as it scans
    *   so that non-options are put at the end. This behavior can be changed
    *   by adding '+' or '-' at the beginning of @a optstring. If the first
    *   character is a '+', argument scanning stops as soon as the first
    *   non-option is encountered. If the character is '-', then non-options
    *   are handled as if they were arguments to the option character code 1.
    *   This is for programs that are written to expect options and other
    *   ARGV-elements in any order and that care about the ordering of the two.
    *   The special argument @c '\--' forces an end of argument scanning regardless
    *   of the ordering.
    *
    *	@par Errors
    *   There are two types of errors in this function: one is an unrecognized
    *   option, and the other is a missing argument to an option. How these are
    *   handle depends on the following.
    *
    *   @par
    *   The default behavior of this function is to print an error message,
    *   place the erroneous option character in the eternal variable @a optopt,
    *   and return '?' to the caller.
    *
    *   @par
    *   The external variable @a opterr can be set to zero to disable all
    *   printing of errors, in which case '?' is returned upon error and it's
    *   up to the caller to figure the rest out. (By default @a opterr is non-zero.)
    *
    *   @par
    *   If the first character in @a optstring (following any optional '+' or '-')
    *   is a colon, then this function likewise does not print an error message.
    *   Instead it returns ':' to indicate a missing argument and '?' for
    *   unrecognized options.
    *
    *   @param argc      The number of elements in @a argv
    *   @param argv      A pointer to an array of strings denoting the commandline arguments
    *   @param optstring A string with a list of legitimate option characters. If
    *                    the character is followed by a ':', that option requires
    *                    an argument. Two colons means that the argument is
    *                    optional. If the first character is '-' or '+', the
    *                    behavior of the function is changed. See section
    *                    @b Ordering above. If the first or second character
    *                    is a colon, errors are not reported. String is of
    *                    the format "[-|+][:]xxxxxxx", where x is any character.
    *   @return          If an option character is found, it is returned. If
    *                    an option character that is not in @a optstring is found,
    *                    '?' is returned. And if an option is missing a required
    *                    argument, then the return value depends on the first
    *                    character in @a optstring: if it is a colon, ':' is
    *                    returned; otherwise, '?' is returned.
    */
    int GetOption(int argc, char** argv, const char* optstring);

    /// @}
}

/// @}

#endif /* CORE_H */
