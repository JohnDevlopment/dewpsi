#ifndef DEWPSI_CAMERA_H
#define DEWPSI_CAMERA_H

/** @file Dewpsi_Camera.h
*   @brief @doxfb
*   Cameras.
*   @defgroup cameras Camera systems
*   @ingroup core
*/

#include <Dewpsi_Core.h>
#include <glm/ext/matrix_transform.hpp>

namespace Dewpsi {
    /** A camera.
    *   @ingroup camera
    */
    class Camera {
    public:
        /// Default constructor.
        Camera() : m_Projection(1.0f) {}

        /// Initialize the projection matrix.
        Camera(const glm::mat4& projection) : m_Projection(projection) {}

        /// Default destructor.
        virtual ~Camera() = default;

    private:
        glm::mat4 m_Projection;
    };
}

#endif /* DEWPSI_CAMERA_H */
