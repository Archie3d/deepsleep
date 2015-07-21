#ifndef DS_SPOTLIGHT_H
#define DS_SPOTLIGHT_H

#include "dsObject.h"

namespace ds {

/**
 * Spot light.
 */
class SpotLight : public Object
{
public:
    SpotLight();

    float cutOffAngle() const { return m_cutOffAngle; }
    float cutOffAngleCos() const { return m_cutOffCos; }
    void setCutOffAngle(float a) { m_cutOffAngle = a; m_cutOffCos = cos(a); }

    glm::vec3 color() const { return m_color; }
    void setColor(const glm::vec3 &c) { m_color = c; }

    bool selectForRendering(Renderer *pRenderer);

private:
    float m_cutOffAngle;    ///< Spot cone cut-off angle.
    float m_cutOffCos;      ///< Spot code angle cosine.
    glm::vec3 m_color;      ///< Light color.
};

} // namespace ds

#endif // DS_SPOTLIGHT_H
