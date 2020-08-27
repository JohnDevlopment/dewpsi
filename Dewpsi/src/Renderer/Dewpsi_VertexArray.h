#ifndef DEWPSI_VERTEXARRAY_H
#define DEWPSI_VERTEXARRAY_H

/**
*   @file Dewpsi_VertexArray.h
*	@brief @doxfb
*   Contains the vertex array definition. Include this file to also include
*   %Dewpsi_Buffer.h.
*	@ingroup renderer
*/

#include <Dewpsi_Memory.h>
#include <Dewpsi_Buffer.h>

namespace Dewpsi {
    /// Vertex array buffer.
    /// @ingroup renderer
    class VertexArray {
    public:
        virtual ~VertexArray() {  }

        /// Bind the vertex array.
        virtual void Bind() const = 0;

        /// Unbind the vertex array.
        virtual void UnBind() const = 0;

        /// Adds a vertex buffer to the vertex array.
        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;

        /// Adds an index buffer to the vertex array.
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        /// Returns the list of vertex buffers.
        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;

        /// Returns the registered index buffer.
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

        /** Creates a vertex array object.
        *   The vertex array is not bound by default.
        */
        static VertexArray* Create();
    };
}

#endif /* DEWPSI_VERTEXARRAY_H */
