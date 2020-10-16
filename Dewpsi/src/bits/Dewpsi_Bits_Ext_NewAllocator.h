#ifndef DEWPSI_BITS_EXT_NEWALLOCATOR_H
#define DEWPSI_BITS_EXT_NEWALLOCATOR_H

/** @file Dewpsi_Bits_Ext_NewAllocator.h
*   @brief An internal header. Do not attempt to use it directly. @doxheader{Dewpsi_Memory.h}
*/

namespace Dewpsi {
#if __cplusplus >= 201703L
    using ::std::align_val_t;
#endif

    #define __AllocatorBase NewAllocator

    template<typename T>
    class NewAllocator {
    public:
        typedef PDsizei __SizeType;
        typedef PDptrdiff __DifferenceType;
        typedef T* __Pointer;
        typedef const T* __ConstPointer;
        typedef T& __Reference;
        typedef const T& __ConstReference;
        typedef T __ValueType;

        template<typename _Tp>
        struct __Rebind {
            typedef NewAllocator<_Tp> other;
        };

#if __cplusplus >= 201103L
        typedef ::std::true_type __PropagateOnContainerMoveAssignment;
#endif

        NewAllocator() noexcept {}

        NewAllocator(const NewAllocator&) noexcept {}

        template<typename _Tp>
        NewAllocator(const NewAllocator<_Tp>&) noexcept {}

        ~NewAllocator() noexcept {}

        /// Return a pointer to @c T.
        __Pointer Address(__Reference x) const noexcept {return AddressOf(x);}

        /// Return a const pointer to @c T.
        __ConstPointer Address(__ConstReference x) const noexcept {return AddressOf(x);}

        /** Allocates a pointer of @a n bytes to @c T.
        *   @param  n   Size of the memory block in bytes
        *   @return     A pointer to a block of memory cast to @c T*
        *   @note       @c T is not constructed or initialized after
        */
        __Pointer Allocate(PDsizei n)
        {
            if (n > this->MaxSize())
                PD_THROW_BAD_ALLOC();

#if __cpp_aligned_new
            if (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            {
                align_val_t al = align_val_t(alignof(T));
                return static_cast<T*>(::operator new(n * sizeof(T), al));
            }
#endif

            return static_cast<T*>(::operator new(n * sizeof(T)));
        }

        /// Free the pointer @a p.
        void Deallocate(__Pointer p, PDsizei)
        {
            if (! p)
                throw ::std::runtime_error("Pointer 'p' is NULL");

    #if __cpp_aligned_new
            if (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            {
                ::operator delete(p, std::align_val_t(alignof(T)));
                return;
            }
    #endif
            ::operator delete(p);
        }

        /// Construct @c _Up with one or more arguments.
        template<typename _Up, typename... _Args>
        void Construct(_Up* p, _Args&&... args)
        {
            ::new ((void*) p) _Up(Forward<_Args>(args)...);
        }

        /// Call the destructor.
        void Destroy(__Pointer p) {p->~T();}

        PDsizei MaxSize() const noexcept {return PDsizei(-1) / sizeof(T);}
    };
}

#endif /* DEWPSI_BITS_EXT_NEWALLOCATOR_H */
