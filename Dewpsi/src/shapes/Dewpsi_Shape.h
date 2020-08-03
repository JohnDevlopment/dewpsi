#ifndef DEWPSI_SHAPE_H
#define DEWPSI_SHAPE_H

/**
*   @file   Dewpsi_Shape.h
*   @brief  @doxfb
*   Defines shapes for both
*/

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /// Generic shape class.
    class Shape {
    public:
        Shape() : m_uiVAO(0), m_uiVBO(0)
        {  }
        virtual ~Shape()
        {  }
        
    protected:
        PDuint m_uiVAO;  ///< Vertex array object
        PDuint m_uiVBO; ///< Vertex buffer object
    };
}

#endif /* DEWPSI_SHAPE_H */
