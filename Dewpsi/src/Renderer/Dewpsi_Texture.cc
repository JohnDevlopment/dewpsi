#include "Dewpsi_Texture.h"

#include "Dewpsi_WhichOS.h"

static PDuint _TexID = 0;

namespace Dewpsi {

Texture::Texture(const PDstring& file) : m_File(file)
{
    stbi_set_flip_vertically_on_load(1);
    m_DataBuffer = stbi_load(file.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    PD_CORE_ASSERT(m_DataBuffer, "Failed to load {0}", file);

    GLCall(glGenTextures(1, &_TexID));
    GLCall(glBindTexture(GL_TEXTURE_2D, _TexID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_DataBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
    glDeleteTextures(1, &_TexID);
    if (m_DataBuffer)
        stbi_image_free(m_DataBuffer);
}

void Texture::Bind(PDuint slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, _TexID));
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
#endif
}

void Texture::UnBind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Ref<Texture> Texture::CreateTexture(const PDstring& file)
{
    return CreateRef<Texture>(file);
}

}
