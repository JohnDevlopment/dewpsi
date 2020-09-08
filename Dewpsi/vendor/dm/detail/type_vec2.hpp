#ifndef TYPE_VEC2_HPP__DEWPSI_INCLUDED
#define TYPE_VEC2_HPP__DEWPSI_INCLUDED

/// @file detail/type_vec2.hpp
/// @ref core

#include "qualifier.hpp"
#include <iostream>

namespace dm {
    /** A vector with two components.
    *   A vector can be used to represent a two-dimensional position/direction.
    *   It can also represent a color. And to facilitate that function, each component
    *   can have one of two names.
    *
    *   @par Data Members
    *   @doxtype{Vec} contains two member variables of type @c T that represent the components
    *   of the vector. Each component can have two different names, depending on the function
    *   of said vector. The data members are part of an anonymous structure within a union.
    *
    *   Position/Direction | Color
    *   ------------------ | -----
    *   x                  | r
    *   y                  | g
    *
    *   @par Example
    *   @code{.cpp}
    *   Vec<2, float> vector1;
    *   vector1.x = 5; // (5, 0)
    *   vector1.y = 6; // (5, 6)
    *   vector1.r = 10; // (10, 6)
    *   vector1.g = 12; // (10, 12)
    *   @endcode
    *
    *   @par Athrimetic Operators
    *   @parblock
        aa
    *   @endparblock
    *
    *   @tparam T A integral or floating-point type
    *   @ingroup core
    */
    template<typename T>
    struct Vec<2, T> {
        typedef T value_type;           ///< Type of the vector components
        typedef Vec<2, T> type;         ///< Type of the vector
        typedef Vec<2, bool> bool_type; ///< Type of the vector if @c T was @c bool

        /// Length type
        typedef length_t length_type;

        Vec();
        Vec(value_type _val);
        Vec(value_type _x, value_type _y);

        ~Vec() = default;

        /// Returns the number of components.
        static DM_CONSTEXPR length_type Length() {return 2;};

        // -- Getter Functions --

        /** Obtain a reference to the x or y component.
        *   Index @a i cannot be negative or greater than 1.
        */
        value_type& operator[](length_type i);

        /** Obtain an immutable reference to the x or y component.
        *   Index @a i cannot be negative or greater than 1.
        */
        const value_type& operator[](length_type i) const;

        // -- Unary Operators --

        /// Assignment operator that accepts a vector.
        Vec& operator=(const Vec& rhs);

        Vec& operator+=(const Vec& rhs);
        Vec& operator+=(T rhs);

        Vec& operator-=(const Vec& rhs);
        Vec& operator-=(T rhs);

        Vec& operator*=(const Vec& rhs);
        Vec& operator*=(T rhs);

        Vec& operator/=(const Vec& rhs);
        Vec& operator/=(T rhs);

        Vec& operator%=(const Vec& rhs);
        Vec& operator%=(T rhs);

        // -- Data --

        union {
            struct {
                T x, y;
            };
            struct {
                T r, g;
            };
        };
    };

    // -- Non-member functions --

    /** Converts a vector from one type to another.
    *   @param  vector  A vector of @c From
    *   @tparam To      @c From is cast to this type
    *   @tparam From    The type of @a vector
    *   @return         A @doxtype{Vec} that's cast from @c From to @c To
    */
    template<typename From, typename To>
    Vec<2, To> vector_cast(const Vec<2, From>& vector);

    template<typename T, typename U = T>
    Vec<2, T> DivideNonZeroV(const Vec<2, T>& lhs, const Vec<2, U>& rhs);

    template<typename T, typename U = T>
    Vec<2, T> DivideNonZeroV(const Vec<2, T>& lhs, U rhs);

    template<typename T, typename U = T>
    Vec<2, T> ModulusNonZeroV(const Vec<2, T>& lhs, const Vec<2, U>& rhs);

    template<typename T, typename U = T>
    Vec<2, T> ModulusNonZeroV(const Vec<2, T>& lhs, U rhs);

    // -- Binary Operators --

    template<typename T>
    Vec<2, T> operator+(const Vec<2, T>& lhs, const Vec<2, T>& rhs);
    template<typename T>
    Vec<2, T> operator+(const Vec<2, T>& lhs, T rhs);
    template<typename T>
    Vec<2, T> operator+(T rhs, const Vec<2, T>& lhs);

    template<typename T>
    Vec<2, T> operator-(const Vec<2, T>& lhs, const Vec<2, T>& rhs);
    template<typename T>
    Vec<2, T> operator-(const Vec<2, T>& lhs, T rhs);
    template<typename T>
    Vec<2, T> operator-(T rhs, const Vec<2, T>& lhs);

    template<typename T>
    Vec<2, T> operator*(const Vec<2, T>& lhs, const Vec<2, T>& rhs);
    template<typename T>
    Vec<2, T> operator*(const Vec<2, T>& lhs, T rhs);
    template<typename T>
    Vec<2, T> operator*(T rhs, const Vec<2, T>& lhs);

    template<typename T>
    Vec<2, T> operator/(const Vec<2, T>& lhs, const Vec<2, T>& rhs);
    template<typename T>
    Vec<2, T> operator/(const Vec<2, T>& lhs, T rhs);
    template<typename T>
    Vec<2, T> operator/(T rhs, const Vec<2, T>& lhs);

    template<typename T>
    Vec<2, T> operator%(const Vec<2, T>& lhs, const Vec<2, T>& rhs);
    template<typename T>
    Vec<2, T> operator%(const Vec<2, T>& lhs, T rhs);
    template<typename T>
    Vec<2, T> operator%(T rhs, const Vec<2, T>& lhs);

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Vec<2, T>& vector)
    {
        os << '(' << vector.x << ", " << vector.y << ')';
        return os;
    }
}

#include "type_vec2.inl"

#endif /* TYPE_VEC2_HPP__DEWPSI_INCLUDED */
