#ifndef DEWPSI_BUFFER_H
#define DEWPSI_BUFFER_H

/** @file       Dewpsi_Buffer.h
*   @brief      @doxfb
*	@ingroup    renderer
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Memory.h>
#include <initializer_list>

namespace Dewpsi {
    /// @addtogroup renderer
    /// @{

    /** Shader data type.
    *   Values of this type must be scoped in @doxtype{ShaderDataType}.
    *   See the example to see it in action:
    *   @code{.cpp}
        Dewpsi::ShaderDataType type = Dewpsi::ShaderDataType::float3;
    *   @endcode
    *   Valid constants of this type are as follows:
    */
    enum class ShaderDataType : PDuint8 {
        Float,    ///< Single float
        Float2,	  ///< Two-component float
        Float3,	  ///< Three-component float
        Float4,	  ///< Four-component float
        Mat3,	  ///< 3x3 matrix
        Mat4,	  ///< 4x4 matrix
        Int,	  ///< Single tnteger
        Int2,	  ///< Two-component tnteger
        Int3,	  ///< Three-component tnteger
        Int4,	  ///< Four-component tnteger
        Bool	  ///< Boolean
	};

    PD_CALL uint32_t ShaderDataTypeSize(ShaderDataType type);

    /** An element of the array held by BufferLayout.
    *   It represents a vertex attribute. A vertec attribute
    *   has a name, a type, a size of that type, and its relative
    *   offset in an attribute.
    */
    struct BufferElement {
        PDstring name;          ///< Name of the element
        ShaderDataType type;    ///< Type of data
        PDuint32 size;          ///< Size of the data type
        PDsizei offset;         ///< The element's offset into in a buffer layout
        PDbool normalized;      ///< If true, normalizes the data

        /// Default constructor.
        BufferElement() = default;

        /// Initialize the type, name and normalization flag of the shader data.
        BufferElement(ShaderDataType _type, const PDstring& _name, bool nml = false)
            : name(_name), type(_type), size(ShaderDataTypeSize(type)),
              offset(0), normalized(nml) {}

        /// Copy constructor.
        BufferElement(const BufferElement& src)
            : name(src.name), type(src.type), size(src.size),
              offset(src.offset), normalized(src.normalized) {  }

        /// Copy assignment.
        BufferElement& operator=(const BufferElement& rhs)
        {
            name = rhs.name;
            type = rhs.type;
            size = rhs.size;
            offset = rhs.offset;
            normalized = rhs.normalized;
            return *this;
        }

        /// Returns the component count.
        PDuint32 GetComponentCount() const;
    };

    /** Buffer layout description.
    *   This is passed to the @doxtype{VertexBuffer} to format its data.
    *   For an example on how to use this class, see the main page.
    */
    class BufferLayout {
        /// Vector type
        typedef std::vector<BufferElement> BufferVector;
    public:
        BufferLayout() = default;

        /// Initialize a list of elements in the layout.
        BufferLayout(const std::initializer_list<BufferElement>& elms);

        /// Copy constructor.
        BufferLayout(const BufferLayout& src)
            : m_Elements(src.m_Elements), m_Stride(src.m_Stride) {}

        /// Copy assignment.
        BufferLayout& operator=(const BufferLayout& rhs)
        {
            m_Elements = rhs.m_Elements;
            m_Stride = rhs.m_Stride;
            return *this;
        }

        /// Return a constant reference to the vector.
        const BufferVector& GetElements() const
        {
            return m_Elements;
        }

        /// Returns the stride of the layout.
        PDuint32 GetStride() const {return m_Stride;}

        /// Returns an iterator to the beginning of the element array.
        BufferVector::iterator begin() { return m_Elements.begin(); }

        /// Returns an iterator to the end of the element array.
        BufferVector::iterator end() { return m_Elements.end(); }

        /// Returns a constant iterator to the beginning of the element array.
        BufferVector::const_iterator begin() const { return m_Elements.begin(); }

        /// Returns a constant iterator to the end of the element array.
        BufferVector::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride();

        BufferVector m_Elements;
        PDuint32 m_Stride;
    };

    /// Vertex buffer.
    class VertexBuffer {
    public:
        VertexBuffer() = default;
        virtual ~VertexBuffer() {}

        /// Bind the vertex buffer.
        virtual void Bind() const = 0;

        /// Unbind the vertex buffer.
        virtual void UnBind() const = 0;

        /// Get the layout of the vertex buffer.
        virtual const BufferLayout& GetLayout() const = 0;

        /// Set the layout of the vertex buffer.
        virtual void SetLayout(const BufferLayout& layout) = 0;

        /** Creates a vertex buffer.
        *   The API-specific vertex buffer is bound (according to the method of the API)
        *   when this is created. Depending on the API, this is neccessary to supply that
        *   API with the data.
        *
        *	@param size  The size of the array pointed to by @a data in bytes
        *	@param data  A pointer to an array of floats that represent vertices
        *	@return      A pointer to the API and platform-specific vertex buffer
        */
        static Ref<VertexBuffer> Create(PDsizei size, const PDfloat* data);
    };

    /// Index array buffer.
    class IndexBuffer {
    public:
        virtual ~IndexBuffer() {  }

        /// Bind the index array.
        virtual void Bind() const = 0;

        /// Unbind the index array.
        virtual void UnBind() const = 0;

        /// Returns the number of indices.
        virtual PDuint32 GetCount() const = 0;

        /** Creates an index buffer.
        *	@param count  The number of elements in the array at @a data
        *	@param data   A pointer to an array of unsigned 32-bit integers,
        *	              used to represent indices into the vertex buffer's data
        *	@return       A pointer to the API and platform-specific vertex buffer
        */
        static Ref<IndexBuffer> Create(PDsizei count, const PDuint32* data);
    };

    /// @}
}

#endif
