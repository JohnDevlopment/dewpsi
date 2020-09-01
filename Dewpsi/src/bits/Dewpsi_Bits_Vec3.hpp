#ifndef DEWPSI_BITS_VEC3_HPP
#define DEWPSI_BITS_VEC3_HPP

#include <Dewpsi_Core.h>

#ifdef _DOXYGEN_DO_INCLUDE
namespace Dewpsi {
#endif /* _DOXYGEN_DO_INCLUDE */

/** @class Dewpsi::TVector3D
*   @brief A 3D vector (x, y, z)
*   @tparam Type An integral or floating-point type
*   @ingroup vectors
*/
template<typename Type>
struct TVector3D {
    /// Constructs an empty vector.
    TVector3D() : x(0), y(0), z(0)
    {  }

    /** Initializes a vector with @a x, @a y, and @a z components.
    *
    *   @param  _x  The X component
    *   @param  _y  The Y component
    *   @param  _z  The Z component
    */
    TVector3D(Type _x, Type _y, Type _z) : x(_x), y(_y), z(_z)
    {  }

    /// Destroys the object.
    ~TVector3D() {}

    /// Copy constructor
    TVector3D(const TVector3D& rhs) : x(rhs.x), y(rhs.y)
    {  }

    /// Move constructor
    TVector3D(TVector3D&& rhs) : x(rhs.x), y(rhs.y)
    {  }

    /** Initialize vector with a list of values.
    *   @snippet vectors.cpp Initialize Vector
    */
    TVector3D(std::initializer_list<Type> il) : x(0), y(0), z(0)
    {
        PDuint i = 0;

        const Type* const end = il.end();
        for (const Type* itr = il.begin(); itr != end; ++itr)
        {
            if (i >= 3) break;
            reinterpret_cast<Type*>(&x)[i++] = *itr;
        }
    }

    /** Add two vectors together.
    *
    *   @snippet vectors.cpp Add Vectors
    */
    TVector3D operator+(const TVector3D& rhs) const
    {
        TVector3D res = {x, y, z};
        res.x += rhs.x;
        res.y += rhs.y;
        res.z += rhs.z;
        return res;
    }

    /** Subtract two vectors.
    *
    *   @snippet vectors.cpp Subtract Vectors
    */
    TVector3D operator-(const TVector3D& rhs) const
    {
        TVector3D res = {x, y, z};
        res.x -= rhs.x;
        res.y -= rhs.y;
        res.z -= rhs.z;
        return res;
    }

    /** Multiply two vectors.
    *
    *   @snippet vectors.cpp Multiply Vectors
    */
    TVector3D operator*(const TVector3D& rhs) const
    {
        TVector3D res = {x, y, z};
        res.x *= rhs.x;
        res.y *= rhs.y;
        res.z *= rhs.z;
        return res;
    }

    /** Divide two vectors.
    *
    *   @snippet vectors.cpp Divide Vectors
    */
    TVector3D operator/(const TVector3D& rhs) const
    {
        TVector3D res = {x, y, z};

        res.x = (res.x && rhs.x) ? (res.x / rhs.x) : 0;
        res.y = (res.y && rhs.y) ? (res.y / rhs.y) : 0;
        res.z = (res.z && rhs.z) ? (res.z / rhs.z) : 0;

        return res;
    }

    /// Compond addition
    TVector3D& operator+=(const TVector3D& rhs) {
        x = x + rhs.x;
        y = y + rhs.y;
        z = z + rhs.z;

    return *this;
    }

    /// Compond subtraction
    TVector3D& operator-=(const TVector3D& rhs) {
        x = x - rhs.x;
        y = y - rhs.y;
        z = z - rhs.z;

    return *this;
    }

    /// Compond multiplication
    TVector3D& operator*=(const TVector3D& rhs) {
        x = x * rhs.x;
        y = y * rhs.y;
        z = z * rhs.z;

    return *this;
    }

    /// Compond division
    TVector3D& operator/=(const TVector3D& rhs) {
        x = (x && rhs.x) ? (x / rhs.x) : 0;
        y = (y && rhs.y) ? (y / rhs.y) : 0;
        z = (z && rhs.z) ? (z / rhs.z) : 0;

    return *this;
    }

    /// Returns true if two vectors are equal.
    bool operator==(const TVector3D& rhs) const {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    /// Returns true if two vectors are not equal.
    bool operator!=(const TVector3D& rhs) const {
        return x != rhs.x && y != rhs.y;
    }

    /** Returns true if the vector is less than @a rhs.
    *   @snippet vectors.cpp Bool Less
    */
    bool operator<(const TVector3D& rhs) const {
        return x < rhs.x && y < rhs.y && z < rhs.z;
    }

    /** Returns true if the vector is less than or equal to @a rhs.
    *   @snippet vectors.cpp Bool LessEqual
    */
    bool operator<=(const TVector3D& rhs) const {
        return x <= rhs.x && y <= rhs.y && z <= rhs.z;
    }

    /** Returns true if the vector is greater than @a rhs.
    *   @snippet vectors.cpp Bool Great
    */
    bool operator>(const TVector3D& rhs) const {
        return x > rhs.x && y > rhs.y && z > rhs.z;
    }

    /** Returns true if the vector is greater than or equal to @a rhs.
    *   @snippet vectors.cpp Bool GreatEqual
    */
    bool operator>=(const TVector3D& rhs) const {
        return x >= rhs.x && y >= rhs.y && z >= rhs.z;
    }

    /// Assignment operator
    TVector3D& operator=(const TVector3D& rhs)
    {
        Copy(rhs);
        return *this;
    }

    /// Assignment operator
    TVector3D& operator=(TVector3D&& rhs)
    {
        Copy(rhs);
        return *this;
    }

    /// Return the length of this vector.
    float Length() const;

    /// Return a normal-length vector.
    TVector3D<float> Normalized() const;

    Type x; ///< X component
    Type y; ///< Y component
    Type z; ///< Z component

    void Copy(const TVector3D& src);
    void Divide(float val);
};

template<typename _Tp>
float TVector3D<_Tp>::Length() const
{
    return (float) std::sqrt(x * x + y * y + z * z);
}

template<typename _Tp>
TVector3D<float> TVector3D<_Tp>::Normalized() const
{
    TVector3D<float> result = {(float) x, (float) y, (float) z};

    result.Divide(result.Length());

    return result;
}

template<typename _Tp>
void TVector3D<_Tp>::Copy(const TVector3D<_Tp>& src)
{
    x = src.x;
    y = src.y;
    z = src.z;
}

template<typename _Tp>
void TVector3D<_Tp>::Divide(float val)
{
    x = (x && val) ? (x / val) : 0;
    y = (y && val) ? (y / val) : 0;
    z = (z && val) ? (z / val) : 0;
}

/** A specialization of TVector3D that utilizes floats
*   @ingroup vectors
*/
typedef TVector3D<float> Vector3D;

/** Constructs a 3D vector with X, Y, and Z components
*   the underlying type of which is @a T.
*
*   @param  x   The X component
*   @param  y   The Y component
*   @param  z   The Z component
*   @return     TVector3D of type @a T
*/
template<typename T>
TVector3D<T> MakeVector3D(T x, T y, T z)
{
    TVector3D<T> vec = {x, y, z};
    return vec;
}

#ifdef _DOXYGEN_DO_INCLUDE
}
#endif /* _DOXYGEN_DO_INCLUDE */

#endif /* DEWPSI_BITS_VEC3_HPP */
