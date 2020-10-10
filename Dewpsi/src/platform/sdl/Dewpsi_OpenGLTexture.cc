#include "Dewpsi_OpenGLTexture.h"
#include "Dewpsi_Log.h"
#include <limits>

#define RESET_ERROR() m_IsError = false;

namespace Dewpsi {

OpenGLTexture2D::OpenGLTexture2D(const PDstring& file) : m_TextureID(0)
{
    RESET_ERROR();
    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID));
    Add(file);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    GLCall(glDeleteTextures(1, &m_TextureID));
}

void OpenGLTexture2D::Bind(PDuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
#endif
}

void OpenGLTexture2D::UnBind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

const PDuchar* OpenGLTexture2D::GetData() const
{
    return nullptr;
}

void OpenGLTexture2D::Add(const PDstring& file)
{
    GLint iWidth, iHeight, iChannels;
    GLenum internalFormat = 0, dataFormat = 0;
    PDuchar* ucpBuffer = nullptr;

    RESET_ERROR();

    // Load file
    stbi_set_flip_vertically_on_load(1);
    ucpBuffer = stbi_load(file.c_str(), &iWidth, &iHeight, &iChannels, 0);
    PD_CORE_ASSERT(ucpBuffer, "Failed to load {0}", file);
    if (! ucpBuffer)
    {
        SetError("Failed to read %s", file);
        m_IsError = true;
        return;
    }

    m_Width  = (GLuint) iWidth;
    m_Height = (GLuint) iHeight;

    // Internal and data format based on the number of channels in the file
    if (iChannels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (iChannels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    PD_CORE_ASSERT(internalFormat && dataFormat, "format not supported");

    PD_CORE_TRACE("This texture will have {0} channels", iChannels);

    // Preallocate storage
    GLCall(glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height));

    // Texture parameters
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Upload pixel data to the current subtexture
    GLCall(glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat,
        GL_UNSIGNED_BYTE, ucpBuffer));
    stbi_image_free(ucpBuffer);
}

}
