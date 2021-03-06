#ifndef DS_POINTLIGHTINGRENDERPASS_H
#define DS_POINTLIGHTINGRENDERPASS_H

#include "dsRenderPass.h"

namespace ds {

class Scene;
class ObjectSelector;
class OpenGlRenderer;
class ShaderProgram;

class PointLightnigRenderPass : public RenderPass
{
public:
    PointLightnigRenderPass(Scene *pScene, OpenGlRenderer *pRenderer);
    ~PointLightnigRenderPass();

    void setColorTexture(const unsigned *pHandle) { m_pColorTexureHandle = pHandle; }
    void setPositionTexture(const unsigned *pHandle) {m_pPositionTextureHandle = pHandle; }
    void setNormalTexture(const unsigned *pHandle) {m_pNormalTextureHandle = pHandle; }

    const unsigned& targetColorTexture() const { return m_colorTexture; }

    void initialize();
    void perform();

private:

    void initializeFbo();

    Scene *m_pScene;
    OpenGlRenderer *m_pRenderer;
    ObjectSelector *m_pPointLightsSelector;
    ShaderProgram *m_pShaderProgram;

    const unsigned *m_pColorTexureHandle;
    const unsigned *m_pPositionTextureHandle;
    const unsigned *m_pNormalTextureHandle;

    bool m_initialized;

    // Render target: FBO
    unsigned m_fbo;
    unsigned m_colorTexture;
    unsigned m_depthBuffer;

    unsigned m_vertexArray;
    unsigned m_vertexBuffer;
    unsigned m_texCoordBuffer;
    unsigned m_elementsBuffer;
};

} // namespace ds

#endif // DS_POINTLIGHTINGRENDERPASS_H
