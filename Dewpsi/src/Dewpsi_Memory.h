#ifndef DEWPSI_MEMORY_H
#define DEWPSI_MEMORY_H

/**
*   @file   Dewpsi_Memory.h
*   @brief  @doxfb
*   Contains memory allocation/manipulation functions.
*   @defgroup memory Memory
*   Memory allocation/manipulation functions.
*   @ingroup core
*/

#include <Dewpsi_Core.h>
#include <memory>

namespace Dewpsi {
    /**
    *   @addtogroup memory
    *   @{
    */

    /// An object that holds a pointer to @c T
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename _Tp>
    struct _MakeScope {
        typedef Scope<_Tp> __single_object;
    };

    template<typename _Tp>
    struct _MakeScope<_Tp[]> {
        typedef Scope<_Tp> __array;
    };

    template<typename _Tp, size_t _Bound>
    struct _MakeScope<_Tp[_Bound]> {
        struct __invalid_type {  };
    };

    /** CreateScope for single objects.
    *   @param args A list of arguments used to construct @c T, or none
    *   @return     A @c %Scope of an object of @c T
    */
    template<typename T, typename... Args>
    inline typename _MakeScope<T>::__single_object
    CreateScope(Args&&... args)
    {
        return Scope<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    inline typename _MakeScope<T>::__single_object
    CreateScope()
    {
        return Scope<T>(new T());
    }

    /** CreateScope for arrays.
    *   @param  num The length of the array
    *   @return     A @c %Scope of an object of @c T
    */
    template<typename T, typename... Args>
    inline typename _MakeScope<T>::__array
    CreateScope(size_t num)
    {
        return Scope<T>(new std::remove_extent_t<T>[num]());
    }

    /// Disable CreateScope for arrays of known bound.
    template<typename T, typename... Args>
    inline typename _MakeScope<T>::__invalid_type
    CreateScope(size_t num) = delete;

    /// A shared reference for an object of @c T
    template<typename T>
    using Ref = std::shared_ptr<T>;

#if __cplusplus >= 201402L
    /** Creates a shared reference to an object of type @c T.
    *   @param args A list of arguments used to construct @c T
    *   @return     A @c %Ref of an object of @c T
    */
    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
#endif

    /// @}
}

#endif /* DEWPSI_MEMORY_H */
