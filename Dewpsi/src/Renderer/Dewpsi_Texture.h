#ifndef DEWPSI_TEXTURE_H
#define DEWPSI_TEXTURE_H

/** @file     Dewpsi_Texture.h
*   @ref      core_renderer_textures
*   @defgroup core_renderer_textures Textures
*   @ingroup  core_renderer
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_String.h>
#include <Dewpsi_Memory.h>
#include <stb_image.h>

namespace Dewpsi {
    /// A texture.
    /// @ingroup core_renderer_textures
    class Texture {
    public:
        Texture() = default;
        virtual ~Texture() = default;

        /// Return an immutable pointer to the pixel data.
        virtual const PDuchar* GetData() const = 0;

        /// Bind the texture to the given slot.
        virtual void Bind(PDuint slot = 0) const = 0;

        /// Unbind the texture.
        virtual void UnBind() const = 0;

        /// Get the width of the texture.
        virtual PDuint GetWidth() const = 0;

        /// Get the height of the texture.
        virtual PDuint GetHeight() const = 0;
    };

    /// A 2D texture.
    /// @ingroup core_renderer_textures
    class Texture2D : public Texture {
    public:
        Texture2D() = default;
        virtual ~Texture2D() = default;

        /// Create a 2D texture from file.
        static Ref<Texture2D> Create(const PDstring& file);
    };
}

#endif /* DEWPSI_TEXTURE_H */
