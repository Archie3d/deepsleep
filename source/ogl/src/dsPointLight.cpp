#include "dsPointLight.h"

namespace ds {

PointLight::PointLight()
    : Object(Type_PointLight),
      m_color(1.0f, 1.0f, 1.0f)
{
}

bool PointLight::selectForRendering(Renderer *pRenderer)
{
    return pRenderer->currentRenderPass()->id() == RenderPass::Id_PointLight;
}

} // namespace ds
