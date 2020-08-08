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

/// A signed integer
typedef int PDint;

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

/// @}

#endif /* DEWPSI_TYPES_H */
