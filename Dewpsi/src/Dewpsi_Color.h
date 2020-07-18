#ifndef DEWPSI_COLOR_H
#define DEWPSI_COLOR_H

/**
*   @file       Dewpsi_Color.h
*   @file       A header for the Dewpsi engine.
*
*   @ingroup    core
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Log.h>
#include <Dewpsi_Math.h>

/// Red green and blue values for the color black
#define PD_COLOR_BLACK  0, 0, 0

namespace Dewpsi {
    /** A structure representing a color.
    *   @ingroup core
    */
    struct Color {
        uint8_t red;    ///< The red component
        uint8_t green;  ///< The green component
        uint8_t blue;   ///< The blue component
        uint8_t alpha;  ///< The alpha component (255 means opaque, 0 is fully transparent)
        
        /// Constructs a Color with default parameters: red, green, blue all zero, but alpha is 255.
        Color() : red(0), green(0), blue(0), alpha(255)
        {  }
        
        /** Constructs a Color with the given color components.
        *   @param  r   Red component
        *   @param  g   Green component
        *   @param  b   Blue component
        *   @param  a   Alpha component (initial value: 255)
        */
        explicit Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
            : red(r), green(g), blue(b), alpha(a)
        {  }
    };
    
    /** A structure representing a color.
    *   @ingroup core
    */
    struct FColor {
        float red;    ///< The red component
        float green;  ///< The green component
        float blue;   ///< The blue component
        float alpha;  ///< The alpha component (255 means opaque, 0 is fully transparent)
        
        /// Constructs an FColor with all-zero values.
        FColor() : red(0.0f), green(0.0f), blue(0.0f), alpha(1.0f)
        {  }
        
        /// Accepts a Color and turns it into an FColor.
        FColor(const Color& color)
            : red((float) color.red / 255.0f),
              green((float) color.green / 255.0f),
              blue((float) color.blue / 255.0f),
              alpha((float) color.alpha / 255.0f)
        {  }
    };
    
    /** Returns a Color structure with the given color components.
    *   @param      red     The red component of a color. Can be 0-255.
    *   @param      green   The green component of a color. Can be 0-255.
    *   @param      blue    The blue component of a color. Can be 0-255.
    *   @note               This function sets the alpha component to 255.
    *   @ingroup    core
    */
    template<typename T>
    inline Color DefineColor(T red, T green, T blue)
    {
        Color temp ((uint8_t) red, (uint8_t) green, (uint8_t) blue, 255);
        return temp;
    }
    
    template<>
    inline Color DefineColor<float>(float red, float green, float blue)
    {
        Color temp((uint8_t) Clamp(0, 255, static_cast<int>(red * 255.0f)),
                    (uint8_t) Clamp(0, 255, static_cast<int>(green * 255.0f)),
                    (uint8_t) Clamp(0, 255, static_cast<int>(blue * 255.0f)),
                    255);
        
        return temp;
    }
    
    /** Returns a Color structure with the given color components.
    *   @param      red     The red component of a color. Can be 0-255.
    *   @param      green   The green component of a color. Can be 0-255.
    *   @param      blue    The blue component of a color. Can be 0-255.
    *   @param      alpha   The alpha component of a color. Can be 0-255.
    *   @ingroup    core
    */
    template<typename T>
    inline Color DefineColor(T red, T green, T blue, T alpha)
    {
        Color temp ((uint8_t) red, (uint8_t) green, (uint8_t) blue, (uint8_t) alpha);
        return temp;
    }
}

inline std::ostream& operator<<(std::ostream& os, const Dewpsi::Color& color)
{
    os << "(r: " << (int) color.red << ", g: " << (int) color.green << ", b: " \
    << (int) color.blue << ", a: " << (int) color.alpha << ')';
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Dewpsi::FColor& color)
{
    os << "(r: " << color.red << ", g: " << color.green << ", b: " \
    << color.blue << ", a: " << color.alpha << ')';
    return os;
}

#endif /* DEWPSI_COLOR_H */
