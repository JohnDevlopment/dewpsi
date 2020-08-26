#ifndef DEWPSI_OPENGLVERTEXARRAY_H
#define DEWPSI_OPENGLVERTEXARRAY_H

#include <Dewpsi_VertexArray.h>
#include <Dewpsi_OpenGL.h>

namespace Dewpsi {
    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override
        {
            return m_VertexBuffers;
        }
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override
        {
            return m_IndexBuffer;
        }

    private:
        PDuint32 m_ArrayID;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}

#endif /* DEWPSI_OPENGLVERTEXARRAY_H */
