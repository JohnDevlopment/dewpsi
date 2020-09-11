#ifndef SETUP_HPP__DEWPSI_INCLUDED

/// @file detail/setup.hpp
/// @ref core

#include <cassert>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

#define DM_VERSION_MAJOR    0
#define DM_VERSION_MINOR    1
#define DM_VERSION_PATCH    0
#define DM_VERSION          DM_VERSION_MAJOR ## DM_VERSION_MINOR ## DM_VERSION_PATCH

#define SETUP_HPP__DEWPSI_INCLUDED DM_VERSION

////////////////////////////////////////////////////////////////////////////////
// 64- or 32-bit machin

/*
#if sizeof(void*) == 8
#   define DM_ARCH  64
#else
#   define DM_ARCH  32
#endif
*/

////////////////////////////////////////////////////////////////////////////////
// C++ version

#define DM_LANG_CXX98_FLAG  (1 << 1)
#define DM_LANG_CXX03_FLAG  (1 << 2)
#define DM_LANG_CXX0X_FLAG  (1 << 3)
#define DM_LANG_CXX11_FLAG  (1 << 4)
#define DM_LANG_CXX14_FLAG  (1 << 5)
#define DM_LANG_CXX17_FLAG  (1 << 6)
#define DM_LANG_CXX2A_FLAG  (1 << 7)
#define DM_LANG_CXXMS_FLAG  (1 << 8)
#define DM_LANG_CXXGNU_FLAG (1 << 9)

#define DM_LANG_CXX98  DM_LANG_CXX98_FLAG
#define DM_LANG_CXX03  (DM_LANG_CXX98_FLAG | DM_LANG_CXX03_FLAG)
#define DM_LANG_CXX0X  (DM_LANG_CXX03 | DM_LANG_CXX0X_FLAG)
#define DM_LANG_CXX11  (DM_LANG_CXX0X | DM_LANG_CXX11_FLAG)
#define DM_LANG_CXX14  (DM_LANG_CXX11 | DM_LANG_CXX14_FLAG)
#define DM_LANG_CXX17  (DM_LANG_CXX14 | DM_LANG_CXX17_FLAG)
#define DM_LANG_CXX2A  (DM_LANG_CXX17 | DM_LANG_CXX2A_FLAG)
#define DM_LANG_CXXMS  DM_LANG_CXXMS_FLAG
#define DM_LANG_CXXGNU DM_LANG_CXXGNU_FLAG

#define DM_LANG_EXT 0

#if __cplusplus > 201703L
#   define DM_LANG  (DM_LANG_CXX2A | DM_LANG_EXT)
#elif __cplusplus == 201703L
#   define DM_LANG  (DM_LANG_CXX17 | DM_LANG_EXT)
#elif __cplusplus == 201402L
#   define DM_LANG  (DM_LANG_CXX14 | DM_LANG_EXT)
#elif __cplusplus == 201103L
#   define DM_LANG  (DM_LANG_CXX11 | DM_LANG_EXT)
#elif __cplusplus == 199711L
#   define DM_LANG  (DM_LANG_CXX98 | DM_LANG_EXT)
#else
#   define DM_LANG  (0 | DM_LANG_EXT)
#endif

////////////////////////////////////////////////////////////////////////////////
// Compiler and language features

#include "platform.h"

#if DM_COMPILER & DM_COMPILER_CLANG
#   define DM_HAS_STATIC_ASSERT __has_feature(cxx_static_assert)
#elif DM_LANG & GLM_LANG_CXX11_FLAG
#   define DM_HAS_STATIC_ASSERT 1
#else
#   define DM_HAS_STATIC_ASSERT 0
#endif

#if DM_LANG & GLM_LANG_CXX11_FLAG
#   define DM_HAS_CONSTEXPR 1
#else
#   define c 0
#endif

#if DM_HAS_CONSTEXPR
#   define DM_CONSTEXPR constexpr
#else
#   define DM_CONSTEXPR
#endif

////////////////////////////////////////////////////////////////////////////////
// Types

#define DM_LENGTH_INT       1
#define DM_LENGTH_SIZE_T    1

#ifdef DM_FORCE_LENGTH_TYPE
#   define DM_CONFIG_LENGTH_TYPE    DM_LENGTH_SIZE_T
#else
#   define DM_CONFIG_LENGTH_TYPE    DM_LENGTH_INT
#endif

namespace dm {
    namespace std {
        using ::std::size_t;

        template<typename T>
        using numeric_limits = ::std::numeric_limits<T>;

        using ::std::uint8_t;
        using ::std::uint16_t;
        using ::std::uint32_t;
        using ::std::uint64_t;

        using ::std::int8_t;
        using ::std::int16_t;
        using ::std::int32_t;
        using ::std::int64_t;

        using ::std::make_unsigned;
        using ::std::ostream;

        using ::std::sqrt;

        using ::std::string;
        using ::std::stringstream;
        using ::std::to_string;
    }

#if DM_CONFIG_LENGTH_TYPE == DM_LENGTH_SIZE_T
    typedef std::size_t length_t;
#else
    typedef int length_t;
#endif
}

#if DM_LANG & DM_LANG_CXX11
#   define DM_HAS_TEMPLATE_ALIASES 1
#else
#   define DM_HAS_TEMPLATE_ALIASES 0
#endif

#if DM_LANG & DM_LANG_CXX11
#   include "traits.hpp"
#endif

#endif /* SETUP_HPP__DEWPSI_INCLUDED */
