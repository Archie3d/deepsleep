#include "dsSpotLight.h"

namespace ds {

SpotLight::SpotLight()
    : Object(Type_SpotLight),
      m_color(1.0f, 1.0f, 1.0f),
      m_cutOffAngle(DS_DEG(5.0f))
{
    m_cutOffCos = cos(m_cutOffAngle);
}

bool SpotLight::selectForRendering(Renderer *pRenderer)
{
    return pRenderer->currentRenderPass()->id() == RenderPass::Id_SpotLight;
}

} // namespace ds
