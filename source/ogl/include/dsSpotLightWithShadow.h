#ifndef DS_SPOTLIGHTWITHSHADOW_H
#define DS_SPOTLIGHTWITHSHADOW_H

#include "dsObject.h"

namespace ds {

class ShaderProgram;

class SpotLightWithShadow : public Object
{
public:

    SpotLightWithShadow();

    float cutOffAngle() const { return m_cutOffAngle; }
    void setCutOffAngle(float a) { m_cutOffAngle = a; }

    glm::vec3 color() const { return m_color; }
    void setColor(const glm::vec3 &c) { m_color = c; }

    bool selectForRendering(Renderer *pRenderer);

    void generateShadowMap();

protected:

    bool initialize();

private:
    float m_cutOffAngle;
    glm::vec3 m_color;

    bool m_initialized;
    unsigned int m_fbo;
    unsigned int m_depthTexture;

    ShaderProgram *m_pShaderProgram;
};

} // namespace ds


#endif // DS_SPOTLIGHTWITHSHADOW_H
