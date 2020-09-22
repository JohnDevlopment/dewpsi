#ifndef CORE_H
#define CORE_H

/**
*   @file       Dewpsi_Core.h
*   @ref        core
*
*   @defgroup   core Core
*   Provisions of the core functionality of Dewpsi.
*   For function attributes, see the following.
*
*   @defgroup platforms Platform-Specific
*   Headers, functions, types, and other declarations specific to each platform.
*/

// Config
#include <Dewpsi_cxx11.h>
#include <bits/Dewpsi_Bits_Attributes.h>

#include <Dewpsi_Util.h>

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
/// @ingroup core
#define PD_MOVE(e)  std::move(e)

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
