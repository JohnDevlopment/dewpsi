#ifndef DEWPSI_BUFFER_H
#define DEWPSI_BUFFER_H

#include <Dewpsi_Core.h>

namespace Dewpsi {
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() {  }

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        static VertexBuffer* Create(PDsizei size, const PDfloat* data);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() {  }

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual PDuint32 GetCount() const = 0;

        static IndexBuffer* Create(PDsizei count, const PDuint32* data);
    };
}

#endif
