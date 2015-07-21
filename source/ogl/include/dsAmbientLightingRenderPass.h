#ifndef DS_AMBIENTLIGHTINGRENDERPASS_H
#define DS_AMBIENTLIGHTINGRENDERPASS_H

#include "dsRenderPass.h"

namespace ds {

class Scene;
class OpenGlRenderer;
class ShaderProgram;
class GeometryRenderPass;

class AmbientLightingRenderPass : public RenderPass
{
public:
    AmbientLightingRenderPass(Scene *pScene, OpenGlRenderer *pRenderer);
    ~AmbientLightingRenderPass();

    void setColorTexture(const unsigned *pHandle) { m_pColorTexture = pHandle; }

    const unsigned& targetColorTexture() const { return m_colorTexture; }

    void initialize();

    void perform();

private:

    void initializeFbo();

    Scene *m_pScene;
    OpenGlRenderer *m_pRenderer;
    ShaderProgram *m_pShaderProgram;

    const unsigned *m_pColorTexture;

    // Render target: FBO
    unsigned m_fbo;
    unsigned m_colorTexture;
    unsigned m_depthBuffer;

    bool m_initialized;

    unsigned m_vertexArray;
    unsigned m_vertexBuffer;
    unsigned m_texCoordBuffer;
    unsigned m_elementsBuffer;
};

} // namespace ds

#endif // DS_AMBIENTLIGHTINGRENDERPASS_H
