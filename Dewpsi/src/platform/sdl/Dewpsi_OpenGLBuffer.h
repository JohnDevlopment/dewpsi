#ifndef DEWPSI_OPENGLBUFFER_H
#define DEWPSI_OPENGLBUFFER_H

#include <Dewpsi_Buffer.h>
#include <Dewpsi_OpenGL.h>

namespace Dewpsi {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(PDsizei size, const PDfloat* data);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

    private:
        PDuint32 m_BufferID;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(PDsizei count, const PDuint32* data);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual PDuint32 GetCount() const override
        {
            return m_Count;
        }

    private:
        PDuint32 m_BufferID;
        PDuint32 m_Count;
    };
}

#endif /* DEWPSI_OPENGLBUFFER_H */
