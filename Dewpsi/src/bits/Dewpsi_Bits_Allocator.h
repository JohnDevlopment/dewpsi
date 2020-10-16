#ifndef DEWPSI_BITS_ALLOCATOR_H
#define DEWPSI_BITS_ALLOCATOR_H

/** @file Dewpsi_Bits_Allocator.h
*   @brief An internal header. Do not attempt to use it directly. @doxheader{Dewpsi_Memory.h}
*/

#define PD_THROW_BAD_ALLOC() \
    throw ::std::bad_alloc();

#include "Dewpsi_Bits_AllocTraits.h"
#include "Dewpsi_Bits_Ext_NewAllocator.h"

namespace Dewpsi {
    /** An allocator.
    *   @tparam  T A type
    *   @ingroup   core_memory
    */
    template<typename T>
    class Allocator : public __AllocatorBase<T> {
    public:
        typedef PDsizei __SizeType;         ///< A type representing sizes
        typedef PDptrdiff __DifferenceType; ///< A type representing differences between pointers
        typedef T* __Pointer;               ///< A pointer to @c T
        typedef const T* __ConstPointer;    ///< A @c const pointer to @c T
        typedef T& __Reference;             ///< A reference to @c T
        typedef const T& __ConstReference;  ///< A @c const reference to @c T
        typedef T __ValueType;              ///< An alias of @c T

        /// Pointer to @c void
        typedef void* __VoidPointer;

        /// Const pointer to @c void
        typedef const void* __ConstVoidPointer;

        /// Rebind the allocator for another type using this struct.
        template<typename _Tp>
        struct __Rebind { typedef Allocator<_Tp> other; };

        typedef TrueType __PropagateOnContainerMoveAssignment;

        typedef TrueType __IsAlwaysEqual;

        /// Default constructor.
        Allocator() throw() {}

        /// Copy constructor.
        Allocator(const Allocator& a) throw() : __AllocatorBase<T>(a) {}

        /// Accept allocator of type @c _Tp.
        template<typename _Tp>
        Allocator(const Allocator<_Tp>&) throw() {}

        /// Destructor
        ~Allocator() throw() {}
    };



    /*
    template<typename _Tp>
      class allocator: public __allocator_base<_Tp>
      {
     public:
        typedef size_t     size_type;
        typedef ptrdiff_t  difference_type;
        typedef _Tp*       pointer;
        typedef const _Tp* const_pointer;
        typedef _Tp&       reference;
        typedef const _Tp& const_reference;
        typedef _Tp        value_type;

        template<typename _Tp1>
      struct rebind
      { typedef allocator<_Tp1> other; };

  #if __cplusplus >= 201103L
        // _GLIBCXX_RESOLVE_LIB_DEFECTS
        // 2103. std::allocator propagate_on_container_move_assignment
        typedef true_type propagate_on_container_move_assignment;

        typedef true_type is_always_equal;
  #endif

        allocator() throw() { }

        allocator(const allocator& __a) throw()
        : __allocator_base<_Tp>(__a) { }

        template<typename _Tp1>
      allocator(const allocator<_Tp1>&) throw() { }

        ~allocator() throw() { }

        // Inherit everything else.
      };
    */

    template<typename T>
    inline bool operator==(const NewAllocator<T>&, const NewAllocator<T>&) {return true;}

    template<typename T>
    inline bool operator!=(const NewAllocator<T>&, const NewAllocator<T>&) {return false;}

    #undef __AllocatorBase
}

#undef PD_THROW_BAD_ALLOC

#endif /* DEWPSI_BITS_ALLOCATOR_H */
