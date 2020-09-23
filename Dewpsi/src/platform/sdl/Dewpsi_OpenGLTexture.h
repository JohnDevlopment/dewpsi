#ifndef DEWPSI_OPENGLTEXTURE_H
#define DEWPSI_OPENGLTEXTURE_H

#include <Dewpsi_Core.h>
#include <Dewpsi_Texture.h>

namespace Dewpsi {
    class OpenGLTexture2D : public Texture2D {
    public:
        explicit OpenGLTexture2D(const PDstring& file);
        virtual ~OpenGLTexture2D();

        virtual const PDuchar* GetData() const override {return m_DataBuffer;}
        virtual void Bind(PDuint slot) const override;
        virtual void UnBind() const override;
        virtual PDuint GetWidth() const override {return m_Width;}
        virtual PDuint GetHeight() const override {return m_Height;}
    private:
        PDuint32 m_TextureID;
        PDint   m_Width;
        PDint   m_Height;
        PDint   m_BPP;
        PDuchar* m_DataBuffer;
    };
}

#endif /* DEWPSI_OPENGLTEXTURE_H */
