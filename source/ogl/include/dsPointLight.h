#ifndef DS_POINTLIGHT_H
#define DS_POINTLIGHT_H

#include "dsObject.h"

namespace ds {

/**
 * Omnidirectional oint light source.
 * This light source does not cast shadows.
 */
class PointLight : public Object
{
public:

    PointLight();

    glm::vec3 color() const { return m_color; }
    void setColor(const glm::vec3 &c) { m_color = c; }

    bool selectForRendering(Renderer *pRenderer);

private:

    /// Light color
    glm::vec3 m_color;
};

} // namespace ds

#endif // DS_POINTLIGHT_H
