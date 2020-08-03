#ifndef DEWPSI_RECT_H
#define DEWPSI_RECT_H

/**
*   @file       Dewpsi_Rect.h
*   @brief      @doxfb
*   Contains data structure for rectangles.
*   @ingroup    core
*/

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /// @addtogroup core
    /// @{
    
    /// A rectangle with integer components
    struct Recti {
        PDint x; ///< X component
        PDint y; ///< Y component
        PDint w; ///< Width
        PDint h; ///< Height
        
        Recti() : x(0), y(0), w(0), h(0)
        {  }
    };
    
    /// A rectangle with unsigned integer components
    struct Rectui {
        PDuint x; ///< X component
        PDuint y; ///< Y component
        PDuint w; ///< Width
        PDuint h; ///< Height
        
        Rectui() : x(0U), y(0U), w(0U), h(0U)
        {  }
    };
    
    /// A rectangle with floating point components
    struct Rectf {
        PDfloat x; ///< X component
        PDfloat y; ///< Y component
        PDfloat w; ///< Width
        PDfloat h; ///< Height
        
        Rectf() : x(0.0f), y(0.0f), w(0.0f), h(0.0f)
        {  }
    };
    
    /** A rectangle that's defined by using two points.
    *   This rectangle is defined by two points, the top-left corner
    *   (the initial xy offset) and the bottom-right corner.
    */
    struct PointRectf {
        PDfloat x1; //< Top-left X coordinate
        PDfloat y1; //< Top-left Y coordinate
        PDfloat x2; //< Bottom-right X coordinate
        PDfloat y2; //< Bottom-right Y coordinate
        
        PointRectf() : x1(0.0f), y1(0.0f), x2(0.0f), y2(0.0f)
        {  }
    };
    
    /// @}
}

#endif /* DEWPSI_RECT_H */
