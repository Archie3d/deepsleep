#include "dsCamera.h"

namespace ds {

Camera::Camera(Scene *pScene)
    : Object(Type_Camera, pScene),
      m_projectionMatrix(1.0f)
{
}

void Camera::setPerspectiveProjection(float viewAngle, float aspectRatio, float nearPlane, float farPlane)
{
    m_projectionMatrix = glm::perspective(viewAngle, aspectRatio, nearPlane, farPlane);
}

void Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
    glm::mat4x4 view = glm::lookAt(eye, center, up);
    setTransformation(glm::inverse(view));
}

const glm::mat4x4& Camera::viewMatrix() const
{
    // Camera view matrix is inverted world matrix of its scene node
    return invertedWorldMatrix();
}

glm::mat4x4 Camera::projectionViewMatrix() const
{
    return m_projectionMatrix * viewMatrix();
}

} // namespace ds
