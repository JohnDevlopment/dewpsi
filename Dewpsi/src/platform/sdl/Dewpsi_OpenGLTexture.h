#ifndef DEWPSI_OPENGLTEXTURE_H
#define DEWPSI_OPENGLTEXTURE_H

#include "Dewpsi_Core.h"
#include "Dewpsi_Texture.h"
#include "Dewpsi_OpenGL.h"
#include "Dewpsi_Vector.h"

namespace Dewpsi {
    struct OpenGLTextureContainer {
        GLuint slot;
        GLint width;
        GLint height;
        GLint channels;
        PDuchar* dataBuffer;

        OpenGLTextureContainer() = default;
        OpenGLTextureContainer(const OpenGLTextureContainer&) = delete;
        OpenGLTextureContainer(OpenGLTextureContainer&& src)
            : slot(src.slot), width(src.width), height(src.height),
              channels(src.channels), dataBuffer(src.dataBuffer)
        {
            src.dataBuffer = nullptr;
        }
    };

    class OpenGLTexture2D : public Texture2D {
    public:
        explicit OpenGLTexture2D(const PDstring& file);
        virtual ~OpenGLTexture2D();

        virtual const PDuchar* GetData() const override {return nullptr;}
        virtual void Bind(PDuint slot) const override;
        virtual void UnBind() const override;
        virtual PDuint GetWidth() const override {return 0;}
        virtual PDuint GetHeight() const override {return 0;}
        virtual void Add(const PDstring& file, PDuint slot) override;
    private:
        GLuint m_TextureID;
        GLuint m_BaseWidth;
        GLuint m_BaseHeight;
        GLuint m_BaseDepth;
        Vector<OpenGLTextureContainer> m_DataBuffers;
    };
}

#endif /* DEWPSI_OPENGLTEXTURE_H */
