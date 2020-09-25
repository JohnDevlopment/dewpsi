#ifndef DEWPSI_UTIL_H
#define DEWPSI_UTIL_H

/** @file                  Dewpsi_Util.h
*   @ref                   core_utility
*   @defgroup core_utility Utility Functions And Macros
*   @ingroup  core
*   @{
*
*   Utility functions for general use by the client application. Some of these
*   macros are also used by the engine.
*
*   Sub-headers:
*   - To include glm headers, define @c PD_INCLUDE_GLM. Further inclusions are:
*       - To include glm type_ptr functions, define @c PD_INCLUDE_GLM_TYPE_PTR
*
*   @warning The macros @doxfunc{PD_CORE_ASSERT} and @doxfunc{PD_ASSERT} rely on the
*            @ref logging "logging functions" to work. Calling them prior to
*            @ref Dewpsi::Log::Init "Log::Init()" will cause a <i>segmentation fault</i>.
*/

#include <Dewpsi_Types.h>
#include <Dewpsi_Log.h>
#include <utility>
#include <Dewpsi_Traits.h>

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
                                        ::Dewpsi::Log::GetClientLogger()->error(\
                                            "Assertion '" #x "' failed | {0}:{1}", __FILE__, __LINE__); \
                                        ::Dewpsi::Log::GetClientLogger()->error(__VA_ARGS__); \
                                        PD_ABORT(); \
                                    }
    #define PD_CORE_ASSERT(x, ...)  if (! (x)) { \
                                        ::Dewpsi::Log::GetCoreLogger()->error(\
                                            "Assertion '" #x "' failed | {0}:{1}", __FILE__, __LINE__); \
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
#define PD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

/** Binds an event to a non-member function.
*   Binds the function @a fn to a function object.
*   @param  fn      The address of a function
*   @param  data    User data (can be @c NULL)
*   @return         An object that can be called with one parameter and that returns @c bool
*/
#define PD_BIND_EVENT_FN_NM(fn, data) std::bind(&fn, std::placeholders::_1)

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
*       PDint8 uiVal = PD_CREATEBYTE(0x0f, 0x01); // results in 0x1f
*       std::cout << std::hex << "0x" << uiVal << std::endl;
*   @endcode
*/
#define PD_CREATEBYTE(lo, hi)   (((PDuint8)(hi) << 4) | (((PDuint8)(lo)) & 0x0f))

/// @}

namespace Dewpsi {
    /// @addtogroup core
    /// @{

    using ::std::remove_reference;
    using ::std::is_lvalue_reference;

    /** Returns an rvalue reference from an lvalue reference.
    *   If the argument is an rvalue reference, it gets returned as such.
    *   @param  src An lvalue or rvalue reference to a value of arbitrary type
    *   @return     The argument cast to an rvalue reference
    */
    template<typename T>
    constexpr T&& Forward(T& src) noexcept {return static_cast<T&&>(src);}
    template<typename T>
    constexpr T&& Forward(T&& src) noexcept {return static_cast<T&&>(src);}

    /** Converts a value to an rvalue reference.
    *   @param src A value of arbitrary type
    *   @return    The value cast to an rvalue reference
    */
    template<typename T>
    constexpr typename remove_reference<T>::type&&
    Move(T&& src) noexcept
    {
        return static_cast<typename remove_reference<T>::type&&>(src);
    }

    /** Fill a range of elements with a certain value.
    *   @param beg,end  The beginning and end elements of a range
    *   @param val      The value to fill the range with
    */
    template<typename _Iterator, typename _Type>
    void Fill(_Iterator beg, _Iterator end, const _Type& val)
    {
        while (beg != end)
            *(beg++) = val;
    }

    template<class _Iterator, class _Function>
    _Function ForEach(_Iterator beg, _Iterator end, _Function fn)
    {
        while (beg != end)
        {
            fn(*beg);
            ++beg;
        }
        return Move(fn);
    }

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

#if defined(__GNUC__)
    template<typename T>
    inline constexpr T* __AddressOf(T& r) {return __builtin_addressof(r);}
#else
    template<typename T>
    inline constexpr T* __AddressOf(T& r) {return &r;}
#endif

    /// Returns the address of @a r.
    template<typename T>
    inline constexpr T* AddressOf(T& r) {return __AddressOf(r);}

    /// @}
}

#endif /* DEWPSI_UTIL_H */
