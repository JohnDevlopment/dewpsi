#ifndef DEWPSI_OBJECTPOOL_HPP
#define DEWPSI_OBJECTPOOL_HPP

/** @ref core
*   @file Dewpsi_ObjectPool.hpp
*/

#include <Dewpsi_Vector.h>

namespace Dewpsi {
    /// An object pool.
    /// @ingroup core
    template<class Object>
    class ObjectPool {
    public:
        ObjectPool() = default;
        ~ObjectPool() = default;

        /// Type of the object
        typedef Object __ObjectType;

        /// Returns an object from the pool.
        __ObjectType* NewObject();

    private:
        Vector<Object*> m_AvailableObjects;
        Vector<Object*> m_UsedObjects;
    };
}

#include "bits/Dewpsi_Bits_ObjectPool-funcs.hpp"

#endif /* DEWPSI_OBJECTPOOL_HPP */
