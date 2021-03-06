#ifndef DEWPSI_ORTHOCAMERA_H
#define DEWPSI_ORTHOCAMERA_H

/** @file Dewpsi_OrthoCamera.h
*   @ref cameras
*/

#include <Dewpsi_Core.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Dewpsi {
    /** An orthographic camera.
    *   @ingroup cameras
    */
    class OrthoCamera {
    public:
        /// Constructs an orthographic camera.
        OrthoCamera(float left, float right, float bottom, float top);

        /// Set the projection of the camera.
        void SetProjection(float left, float right, float bottom, float top);

        /// Returns the position of the camera.
        const glm::vec3& GetPosition() const {return m_Position;}

        /// Sets the position of the camera.
        void SetPosition(const glm::vec3& pos)
        {
            m_Position = pos;
            RecalculateViewMatrix();
        }

        /// Set the angle of the rotation.
        /// The angle is in degrees.
        void SetRotation(float angle) {m_Rotation = angle;}

        /// Get the angle of the rotation.
        float GetRotation() const {return m_Rotation;}

        /// Returns the view matrix.
        const glm::mat4& GetViewMatrix() const {return m_ViewMatrix;}

        /// Returns the projection matrix.
        const glm::mat4& GetProjectionMatrix() const {return m_ProjectionMatrix;}

        /// Returns the view-projection matrix.
        const glm::mat4& GetViewProjectionMatrix() const {return m_ViewProjectionMatrix;}

    private:
        void RecalculateViewMatrix();

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position;
        float m_Rotation;
    };
}

#endif /* DEWPSI_ORTHOCAMERA_H */
