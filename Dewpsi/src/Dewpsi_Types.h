#ifndef DEWPSI_TYPES_H
#define DEWPSI_TYPES_H

/**
*   @file   Dewpsi_Types.h
*   @brief  A header for the Dewpsi engine.
*   Contains typedefs and forward declarations.
*
*   @ingroup core
*   @{
*/

#include <cstdint>
#include <string>
#include <limits>
#include <initializer_list>
#include <new>

/// A signed integer type that represents the difference between pointers
typedef ptrdiff_t PDptrdiff;

/// An unsigned type used to represent the size of objects
typedef size_t PDsizei;

/// Used to specify the alignment of values
enum class PDalignval : PDsizei {};

/// A signed integer
typedef int PDint;

/// A signed long integer
typedef long int PDlong;

/// A signed double-long integer
typedef long long PDllong;

/// A signed short integer
typedef short int PDshort;

/// A character
typedef char PDchar;

/// A floating point
typedef float PDfloat;

/// A signed 32 bit integer
typedef int32_t PDint32;

/// A signed 16 bit integer
typedef int16_t PDint16;

/// A signed 8 bit integer
typedef int8_t PDint8;

/// An unsigned integer
typedef unsigned int PDuint;

/// An unsigned long integer
typedef unsigned long int PDulong;

/// An unsigned double-long integer
typedef unsigned long long PDullong;

/// An unsigned short
typedef unsigned short PDushort;

/// An unsigned character
typedef unsigned char PDuchar;

/// An unsigned 32 bit integer
typedef uint32_t PDuint32;

/// An unsigned 16 bit integer
typedef uint16_t PDuint16;

/// An unsigned 8 bit integer
typedef uint8_t PDuint8;

/// A boolean true or false type
typedef bool PDbool;

/// Values of the type PDbool
enum PDboolEnum : PDbool {
    PD_TRUE  = true,    ///< True value
    PD_FALSE = false    ///< False value
};

/// For enumerated values
enum PDenum : PDuint32 {
    PD_INVALID  = 0xffffffff, ///< Invalid; meaning depends on function
    PD_NULL     = 0x00000000, ///< NULL value
    PD_ALLOC    = 0x00000001, ///< Allocate a new array
    PD_OKAY     = 0x00000002  ///< Okay, function returned successfully
};

/// User data type
typedef void* PDuserdata;

/// String class
typedef std::string PDstring;

namespace Dewpsi {
    using ::std::numeric_limits;
    using ::std::initializer_list;

    /** Provides information about the properties of arithmetic types (either integral
    *   or floating-point) in the specific platform for which the library compiles.
    *   This class template is specialized for every fundamental arithmetic type,
    *   with its members describing the properties of type T. This template shall
    *   not be specialized for any other type.
    *
    *   @note
    *   This class is inherited from std::numeric_limits and provides specializations for
    *   Dewpsi types that are just typedefs for their respective arithmetic types.
    */
    template<typename T>
    class NumericLimits : public numeric_limits<T> {};
    template<>
    class NumericLimits<PDuint> : public numeric_limits<unsigned int> {};

    /// An alias of std::initializer_list.
    template<typename T>
    using InitList = initializer_list<T>;
}

/*Provides information about the properties of arithmetic types (either integral or floating-point) in the specific platform for which the library compiles.

This class template is specialized for every fundamental arithmetic type, with its members describing the properties of type T. This template shall not be specialized for any other type.*/

/// @}

#endif /* DEWPSI_TYPES_H */
