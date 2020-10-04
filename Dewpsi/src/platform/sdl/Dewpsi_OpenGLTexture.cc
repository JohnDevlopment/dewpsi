#include "Dewpsi_OpenGLTexture.h"
#include "Dewpsi_Log.h"

namespace Dewpsi {

OpenGLTexture2D::OpenGLTexture2D(const PDstring& file) : m_TextureID(0)
{
    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID));
    Add(file, 0);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_TextureID);
}

void OpenGLTexture2D::Bind(PDuint slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
#endif
}

void OpenGLTexture2D::UnBind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void OpenGLTexture2D::Add(const PDstring& file, PDuint slot)
{
    GLint iWidth, iHeight, iDepth;
    GLenum internalFormat = 0, dataFormat = 0;
    PDuchar* ucpBuffer = nullptr;

    // Load file
    stbi_set_flip_vertically_on_load(1);
    ucpBuffer = stbi_load(file.c_str(), &iWidth, &iHeight, &iDepth, 0);
    if (! ucpBuffer)
    {
        SetError("Failed to read %s", file);
        return;
    }

    // Internal and data format based on the number of channels in the file
    if (iDepth == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (iDepth == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    PD_CORE_ASSERT(internalFormat && dataFormat, "format not supported");

    // Bind texture
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

    // Texture parameters
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Upload pixel data to the current subtexture
    GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, iWidth, iHeight, dataFormat,
        GL_UNSIGNED_BYTE, ucpBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

}
