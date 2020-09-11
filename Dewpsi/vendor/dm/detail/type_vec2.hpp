#ifndef TYPE_VEC2_HPP__DEWPSI_INCLUDED
#define TYPE_VEC2_HPP__DEWPSI_INCLUDED

/// @file detail/type_vec2.hpp
/// @ref core

#include "qualifier.hpp"
#include <iostream>
#include <string>
#include <cmath>

namespace dm {
    /** A vector with two components.
    *   A vector can be used to represent a two-dimensional position/direction.
    *   It can also represent a color. And to facilitate that function, each component
    *   can have one of two names.
    *
    *   The x component can be obtained through the members @a x or @a r. The y component
    *   can be obtained through the members @a y or @a g.
    *
    *   @par Constructors
    *   @include vec_ctor.cpp
    *   -# Initialize the vector to zero
    *   -# Initialize the vector to @a val
    *   -# Initialize the vector to @a x and @a y
    *
    *   @par Vector Athrimetic
    *   @include vec_binOps.cpp
    *
    *   @par Relational Operators
    *   Two vectors can be compared with each other using the ==, !=, <=, >=, <, and > operators.
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

        Vec(const Vec& src);

        ~Vec() = default;

        /// Returns the number of components.
        static DM_CONSTEXPR length_type Count() {return 2;};

        // -- Getter Functions --

        /** Obtain a reference to the x or y component.
        *   Index @a i cannot be negative or greater than 1.
        */
        value_type& operator[](length_type i);

        /** Obtain an immutable reference to the x or y component.
        *   Index @a i cannot be negative or greater than 1.
        */
        const value_type& operator[](length_type i) const;

        // -- Relational Operators --

        bool operator==(const Vec& b) const;
        bool operator!=(const Vec& b) const;
        bool operator<=(const Vec& b) const;
        bool operator>=(const Vec& b) const;
        bool operator<(const Vec& b) const;
        bool operator>(const Vec& b) const;

        // -- Unary Operators --

        /// Assignment operator that accepts a vector.
        Vec& operator=(const Vec& rhs);

        /// Assignment operator that accepts a vector.
        Vec& operator=(Vec&& rhs);

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

        // -- Vector functions --

        /// Returns the length of the vector.
        float Length() const;

        /// Retrieve a string with the value of the vector.
        std::string GetString() const;

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

    template<typename T>
    Vec<2, T> normalize(const Vec<2, T>& v);

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

    // -- Output for vector --

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Vec<2, T>& vector)
    {
        os << '(' << vector.x << ", " << vector.y << ')';
        return os;
    }
}

#include "type_vec2.inl"

#endif /* TYPE_VEC2_HPP__DEWPSI_INCLUDED */
