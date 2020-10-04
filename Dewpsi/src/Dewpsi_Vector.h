#ifndef DEWPSI_VECTOR_H
#define DEWPSI_VECTOR_H

/// @ref core
/// @file Dewpsi_Vector.h

#include <Dewpsi_Core.h>
#include <Dewpsi_Math.h>
#include <Dewpsi_Traits.h>
#include <Dewpsi_Iterator.h>
#include <Dewpsi_Except.h>
#include <cassert>
#include <initializer_list>
#include <tuple>

namespace Dewpsi {
    /** A vector container.
    *   @tparam T Type of the elements of @doxtype{Vector}
    */
    template<typename T>
    class Vector {
    public:
        typedef T __ValueType; ///< Element type
        typedef T* __Pointer; ///< Pointer to @c T
        typedef const T* __ConstPointer; ///< Const pointer to @c T
        typedef T& __Reference; ///< Reference of type @c T
        typedef const T& __ConstReference; ///< Const reference of type @c T

        enum __ArrayContainerEnum {_A, _B};

        struct __ArrayContainer {
            __Pointer begin, finish, capacityEnd;

            __ArrayContainer() : begin(nullptr), finish(nullptr), capacityEnd(nullptr) {}

            __ArrayContainer(const __ArrayContainer& src)
                : begin(src.begin), finish(src.finish), capacityEnd(src.capacityEnd) {}

            __ArrayContainer(__ArrayContainer&& src)
                : begin(src.begin), finish(src.finish), capacityEnd(src.capacityEnd)
            {
                src.begin = src.finish = src.capacityEnd = nullptr;
            }

            __ArrayContainer(__Pointer b, __Pointer f, __Pointer e)
                : begin(b), finish(f), capacityEnd(e) {}

            void operator=(__ArrayContainer&& rhs)
            {
                begin = rhs.begin;
                finish = rhs.finish;
                capacityEnd = rhs.capacityEnd;

                rhs.begin = nullptr;
                rhs.finish = nullptr;
                rhs.capacityEnd = nullptr;
            }

            PDsizei Size() const {return static_cast<PDsizei>(finish - begin);}
            PDsizei Capacity() const {return static_cast<PDsizei>(capacityEnd - begin);}
        };

        /// Iterator that points to @c T
        typedef T* __Iterator;

        /// Readonly iterator that points to @c T
        typedef const T* __ConstIterator;

        /// Reverse iterator
        typedef Dewpsi::ReverseIterator<__Iterator> __ReverseIterator;

        /// Const reverse iterator
        typedef Dewpsi::ReverseIterator<__ConstIterator> __ConstReverseIterator;

        constexpr Vector() : m_Begin(nullptr), m_Finish(nullptr), m_CapacityEnd(nullptr) {}

        /// Initialize the vector with a size of @a n.
        explicit Vector(PDsizei n) : m_Begin(nullptr), m_Finish(nullptr),
            m_CapacityEnd(nullptr) {Resize(n);}

        /// Initialize the vector with a list of values.
        Vector(const std::initializer_list<T>& il) {SetData(il);}

        /// Destroys the vector.
        ~Vector() {Destroy();}

        __Iterator begin() {return m_Begin;}
        __ConstIterator begin() const {return m_Begin;}
        __Iterator end() {return m_Finish;}
        __ConstIterator end() const {return m_Finish;}

        /// Returns a const/non-const reverse iterator to the end of the array.
        __ReverseIterator rbegin() {return __ReverseIterator(this->begin());}
        __ConstReverseIterator rbegin() const {return __ConstReverseIterator(this->begin());}
        __ReverseIterator rend() {return __ReverseIterator(this->end());}
        __ConstReverseIterator rend() const {return __ConstReverseIterator(this->end());}

        /// Return a reference to the element @a n of the vector.
        __Reference operator[](PDsizei n)
        {
            assert(n < this->Size());
            return m_Begin[n];
        }

        /// Return a @c const reference to the element @a n of the vector.
        __ConstReference operator[](PDsizei n) const
        {
            assert(n < this->Size());
            return m_Begin[n];
        }

        /// Return a reference to the element @a n of the vector.
        /// @throw std::out_of_range Parameter @a n is outside the vector's range
        __Reference At(PDsizei n)
        {
            if (n >= this->Size())
            {
                SetError("Param 'n' (%u) outside range (%u)...", n, this->Size());
                throw std::out_of_range(GetError());
            }
            return m_Begin[n];
        }

        /// Return a @c const reference to the element @a n of the vector.
        /// @throw std::out_of_range Parameter @a n is outside the vector's range
        __ConstReference At(PDsizei n) const
        {
            if (n >= this->Size())
            {
                SetError("Param 'n' (%u) outside range (%u)...", n, this->Size());
                throw std::out_of_range(GetError());
            }
            return m_Begin[n];
        }

        /// Resize the array to @a n elements.
        void Resize(PDsizei n);

        /// Returns the size of the vector.
        PDsizei Size() const noexcept {return static_cast<PDsizei>(m_Finish - m_Begin);}

        /// Returns the capacity of the vector.
        PDsizei Capacity() const noexcept {return static_cast<PDsizei>(m_CapacityEnd - m_Begin);}

        /// Return a raw pointer to the array.
        __Pointer Data() {return m_Begin;}

        /// Return a @c const raw pointer to the array.
        __ConstPointer Data() const {return m_Begin;}

        /// Adds a new element to the end of the vector, after the last element.
        void PushBack(const __ValueType& src);

        /// Adds a new element to the end of the vector, after the last element.
        void PushBack(__ValueType&& src);

        /// Sets elements of @doxtype{Vector} according to a list.
        void SetData(const std::initializer_list<T>& il);

    private:
        __Pointer m_Begin;
        __Pointer m_Finish;
        __Pointer m_CapacityEnd;

        /// Destroys the array and clears the pointers.
        void Destroy();

        /// Initialize some values for @doxfunc{PushBack}.
        void InitArrayContainers(__ArrayContainer& A, __ArrayContainer& B, PDsizei& index) const;

        /// Initialize some values for @doxfunc{Resize}.
        void InitArrayContainers(__ArrayContainer& A, __ArrayContainer& B, PDsizei newSize,
            PDsizei newCapacity) const;

        /// Private allocation function.
        static __Pointer Allocate(PDsizei n);

        /// Returns a suggested number for the new array size.
        static PDsizei NewArraySize(PDsizei n);

        /// Move contents of one array to the other.
        static void MoveContents(__ArrayContainer& dst, __ArrayContainer& src);
    };

    /// @cond NEVER
    template<typename Tp>
    void Vector<Tp>::Destroy()
    {
        if (this->Capacity() > 1)
            delete[] m_Begin;
        else
            delete m_Begin;
        m_Begin = m_Finish = m_CapacityEnd = nullptr;
    }

    template<typename Tp>
    void Vector<Tp>::InitArrayContainers(Vector<Tp>::__ArrayContainer& A,
        Vector<Tp>::__ArrayContainer& B, PDsizei& index) const
    {
        const PDsizei sizeCapacity[2][2] = {
            { A.Size(), A.Capacity() },
            { B.Size(), B.Capacity() }
        };

        PD_CORE_TRACE("sizeCapacity:\nA.size = {} A.cap = {}\nB.size = {} B.cap = {}",
            sizeCapacity[_A][0], sizeCapacity[_A][1], sizeCapacity[_B][0], sizeCapacity[_B][1]);

        // Empty vector initialized to two capacity
        if (! sizeCapacity[_A][1])
        {
            B.begin       = Allocate(2U);
            B.finish      = B.begin + 1U;
            B.capacityEnd = B.begin + 2U;
            index         = 0U;
        }

        // Insert at end of array
        else
        {
            // Allocate new array if A.size == A.capacity (no room left)
            if (sizeCapacity[_A][0] == sizeCapacity[_A][1])
            {
                // Destination array
                const PDsizei newSize = this->NewArraySize(sizeCapacity[_A][1]);
                B.begin = Allocate(newSize);
                B.finish = B.begin + sizeCapacity[_A][0]; // Size = A.size
                B.capacityEnd = B.begin + newSize; // Capacity = 'newSize'
                MoveContents(B, A);
                if (sizeCapacity[_A][1] == 1)
                    delete A.begin;
                else
                    delete[] A.begin;
            }

            index = sizeCapacity[_A][0]; // Index = A.size
            B.finish++;
        }
    }

    template<typename Tp>
    void Vector<Tp>::InitArrayContainers(Vector<Tp>::__ArrayContainer& A,
        Vector<Tp>::__ArrayContainer& B, PDsizei newSize, PDsizei newCapacity) const
    {
        const PDsizei oldSize     = A.Size();
        const PDsizei oldCapacity = A.Capacity();

        // Nop if the requested is the current size
        if (newSize == oldSize)
            return;

        // If the requested size of the array is less than the current capacity, set the new
        // capacity to be equal to the current capacity, seeing as there is no need to resize
        // the array.
        if (newSize <= oldCapacity)
            newCapacity = oldCapacity;

        // If the requested capacity is greater than the current capacity, then
        // the array must be resized.
        assert(newCapacity >= oldCapacity);
        if (newCapacity > oldCapacity)
        {
            B.begin = Allocate(newCapacity);
            B.finish = B.begin + oldSize;
            B.capacityEnd = B.begin + newCapacity;
            MoveContents(B, A);
            B.finish = B.begin + newSize;
        }

        // Move the end pointer.
        if (newSize > oldSize)
            B.finish = B.begin + newSize;
    }

    template<typename Tp>
    void Vector<Tp>::Resize(PDsizei n)
    {
        __ArrayContainer sourceArray(m_Begin, m_Finish, m_CapacityEnd);
        __ArrayContainer destArray;
        PDsizei newCapacity;

        // Nonnull pointer to array
        if (sourceArray.begin)
        {
            const PDsizei oldCapacity = sourceArray.Capacity();

            // The array must be enlarged if the requested size goes over the old capacity
            if (n > oldCapacity)
                newCapacity = oldCapacity + 3 /*NewArraySize(oldCapacity)*/;

            // But in the alternative where N is less than or equal to the old capacity, the new
            // capacity is equal to the old capacity.
            else
                newCapacity = oldCapacity;
        }
        else
        {
            // Null pointer to array. N is the new capacity of the array.
            newCapacity = n;
        }

        InitArrayContainers(sourceArray, destArray, n, newCapacity);
        m_Begin       = destArray.begin;
        m_Finish      = destArray.finish;
        m_CapacityEnd = destArray.capacityEnd;
    }

    template<typename Tp>
    void Vector<Tp>::PushBack(const Vector<Tp>::__ValueType& src)
    {
        __ArrayContainer sourceArray(m_Begin, m_Finish, m_CapacityEnd);
        __ArrayContainer destArray = sourceArray;
        PDsizei index;

        InitArrayContainers(sourceArray, destArray, index);

        m_Begin = destArray.begin;
        m_Finish = destArray.finish;
        m_CapacityEnd = destArray.capacityEnd;

        // Copy SRC to the array
        m_Begin[index] = src;
    }

    template<typename Tp>
    void Vector<Tp>::PushBack(Vector<Tp>::__ValueType&& src)
    {
        __ArrayContainer sourceArray(m_Begin, m_Finish, m_CapacityEnd);
        __ArrayContainer destArray = sourceArray;
        PDsizei index;

        InitArrayContainers(sourceArray, destArray, index);

        m_Begin = destArray.begin;
        m_Finish = destArray.finish;
        m_CapacityEnd = destArray.capacityEnd;

        // "Move" SRC to the array
        m_Begin[index] = Forward(src);
    }
    /// @endcond

    template<typename Tp>
    void Vector<Tp>::SetData(const std::initializer_list<Tp>& il)
    {
        Resize(il.size());
        __Iterator iter = this->begin();
        for (auto value : il)
            *iter++ = value;
    }

    template<typename Tp>
    typename Vector<Tp>::__Pointer Vector<Tp>::Allocate(PDsizei n)
    {
        assert(n > 0);
        __Pointer elements = (n > 1) ? new Tp[n] : new Tp;
        return elements;
    }

    template<typename Tp>
    PDsizei Vector<Tp>::NewArraySize(PDsizei n)
    {
        const double dLog = std::log10(n);
        double dMultiplier = (dLog >= 1.0) ? dLog : (1.0 + dLog);
        return static_cast<PDsizei>((double) max(n, PDsizei(1)) * dMultiplier + 1);
    }

    // TODO #1: Add some kind of vector traits class the decides whether to move or copy src.begin[i]
    template<typename Tp>
    void Vector<Tp>::MoveContents(Vector<Tp>::__ArrayContainer& dst, Vector<Tp>::__ArrayContainer& src)
    {
#if 1
        const PDsizei count = min(src.Size(), dst.Size());
        for (PDsizei i = 0; i < count; ++i)
            dst.begin[i] = PD_MOVE(src.begin[i]);
#endif
#if 0
        typedef VectorTraits<Vector<Tp>> __VectorTraits;
        for (PDsizei i = 0; i < count; ++i)
            dst.begin[i] = __VectorTraits::Assign(src.begin[i]);
#endif
    }
}

#endif /* DEWPSI_VECTOR_H */
