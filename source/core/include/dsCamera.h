#ifndef DS_CAMERA_H
#define DS_CAMERA_H

#include "dsObject.h"

namespace ds {

/**
 * Camera used to project a scene.
 */
class Camera : public Object
{
public:

    Camera(Scene *pScene = nullptr);

    const glm::mat4x4& projectionMatrix() const { return m_projectionMatrix; }
    void setProjectionMatrix(const glm::mat4x4 &m) { m_projectionMatrix = m; }
    void setPerspectiveProjection(float viewAngle, float aspectRatio, float nearPlane, float farPlane);
    void lookAt(const glm::vec3 &eye,
                const glm::vec3 &center,
                const glm::vec3 &up);
    const glm::mat4x4& viewMatrix() const;
    glm::mat4x4 projectionViewMatrix() const;

private:

    /// Projection matrix.
    glm::mat4x4 m_projectionMatrix;
};

} // namespace ds

#endif // DS_CAMERA_H
