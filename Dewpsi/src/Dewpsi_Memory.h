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
        typedef Scope<_Tp[]> __array;
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

    template<typename _Tp>
    struct _MakeRef {
        typedef Ref<_Tp> __single_object;
    };

    template<typename _Tp>
    struct _MakeRef<_Tp[]> {
        typedef Ref<_Tp[]> __array;
    };

    template<typename _Tp, size_t _Bound>
    struct _MakeRef<_Tp[_Bound]> {
        struct __invalid_type {  };
    };

    /** Construct a referenced object with optional parameters.
    *   @param  args    An optional list of arguments to initialize the constructed object
    *   @return         A @doxtype{Ref} of type @c T
    *   @tparam T       The type of the constructed object
    */
    template<typename T, typename... Args>
    inline typename _MakeRef<T>::__single_object
    CreateRef(Args&&... args)
    {
        return Ref<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args>
    inline typename _MakeRef<T>::__single_object
    CreateRef()
    {
        return Ref<T>(new T);
    }

    /** Construct a referenced array.
    *   @param  num The number of elements
    *   @return     A @doxtype{Ref} of array type @c T
    *   @tparam T   An array type of known bound
    */
    template<typename T, typename... Args>
    inline typename _MakeRef<T>::__array
    CreateRef(size_t num)
    {
        return Ref<T>(new std::remove_extent_t<T>[num]);
    }

    template<typename T, typename... Args>
    inline typename _MakeRef<T>::__invalid_type
    CreateRef(size_t num) = delete;

    /// An overload of @doxfunc{ResetRefs} with no arguments.
    inline void ResetRefs() {}

    /** Call this function to release a list of referenced objects.
    *   @param  r       A referenced object of arbitrary type @c T
    *   @param  args    An optional list of arguments to include
    *   @tparam T       Any type supported by @doxtype{Ref}
    *   @note           Each argument must be a mutable l-value reference to @doxtype{Ref}.
    */
    template<typename T, typename... Args>
    void ResetRefs(Ref<T>& r, Args... args)
    {
        r.reset();
        //ResetRefs(r);
        ResetRefs(args...);
    }

    // Casting operations

    /** Casts a reference object to a different type via static_cast.
    *   @param  src The Ref to be cast, of type @c U
    *   @return     A Ref of type @c T, converted from @c U
    *   @tparam T,U The types converted to and from, respectively
    */
    template<typename T, typename U>
    inline Ref<T> static_ref_cast(const Ref<U>& src)
    {
        using _ref = Ref<T>;
        return _ref(src, static_cast<typename _ref::element_type*>(src.get()));
    }

    /// @}
}

#endif /* DEWPSI_MEMORY_H */
