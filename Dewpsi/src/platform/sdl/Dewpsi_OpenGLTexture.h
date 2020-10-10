#ifndef DEWPSI_OPENGLTEXTURE_H
#define DEWPSI_OPENGLTEXTURE_H

#include "Dewpsi_Core.h"
#include "Dewpsi_Texture.h"
#include "Dewpsi_OpenGL.h"
#include "Dewpsi_Vector.h"

namespace Dewpsi {
    /*struct OpenGLTextureAttributes {
        GLuint   slot;
        GLint    width;
        GLint    height;
        GLint    channels;
        PDuchar* dataBuffer;

        OpenGLTextureAttributes() = default;
        //OpenGLTextureAttributes(const OpenGLTextureAttributes&) = delete;
        OpenGLTextureAttributes(OpenGLTextureAttributes&& src)
            : slot(src.slot), width(src.width), height(src.height),
              channels(src.channels), dataBuffer(src.dataBuffer)
        {
            src.dataBuffer = nullptr;
        }

        OpenGLTextureAttributes& operator=(OpenGLTextureAttributes&& src)
        {
            slot = src.slot;
            width = src.width;
            height = src.height;
            channels = src.channels;
            dataBuffer = src.dataBuffer;
            src.dataBuffer = nullptr;
            return *this;
        }

        ~OpenGLTextureAttributes()
        {
            delete dataBuffer;
            dataBuffer = nullptr;
        }
    };*/

    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D() = delete;
        explicit OpenGLTexture2D(const PDstring& file);
        virtual ~OpenGLTexture2D();

        virtual void Bind(PDuint slot) const override;
        virtual void UnBind() const override;
        virtual PDuint GetWidth() const override {return static_cast<GLuint>(m_Width);}
        virtual PDuint GetHeight() const override {return static_cast<GLuint>(m_Height);}
        virtual const PDuchar* GetData() const override;

        void Add(const PDstring& file);
    private:
        GLuint m_TextureID;
        GLuint m_Width;
        GLuint m_Height;
    };
}

#endif /* DEWPSI_OPENGLTEXTURE_H */
