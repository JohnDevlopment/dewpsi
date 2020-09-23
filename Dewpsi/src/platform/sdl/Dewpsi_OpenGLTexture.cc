#include "Dewpsi_OpenGLTexture.h"
#include "Dewpsi_Log.h"
#include "Dewpsi_WhichOS.h"

namespace Dewpsi {

OpenGLTexture2D::OpenGLTexture2D(const PDstring& file)
{
    stbi_set_flip_vertically_on_load(1);
    m_DataBuffer = stbi_load(file.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    PD_CORE_ASSERT(m_DataBuffer, "Failed to load {0}", file);

    GLCall(glGenTextures(1, &m_TextureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_DataBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_TextureID);
    if (m_DataBuffer)
        stbi_image_free(m_DataBuffer);
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

}
