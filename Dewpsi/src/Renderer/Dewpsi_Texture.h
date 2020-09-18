#ifndef DEWPSI_TEXTURE_H
#define DEWPSI_TEXTURE_H

/// @file Dewpsi_Texture.h
/// @ref core

#include <Dewpsi_Core.h>
#include <Dewpsi_String.h>
#include <Dewpsi_Memory.h>
#include <stb_image.h>

namespace Dewpsi {
    /** A texture.
    *   @ingroup core
    */
    class Texture {
    public:
        /// Construct a texture of the given file.
        Texture(const PDstring& file);
        ~Texture();

        /// Return an immutable pointer to the pixel data.
        const PDuchar* GetData() const {return m_DataBuffer;}

        void Bind(PDuint slot = 0) const;
        void UnBind() const;

        /// Create a new texture based on @a file.
        static Ref<Texture> CreateTexture(const PDstring& file);
    private:
        PDstring m_File;
        PDuchar* m_DataBuffer;
        int m_Width;
        int m_Height;
        int m_BPP;
    };
}

#endif /* DEWPSI_TEXTURE_H */
