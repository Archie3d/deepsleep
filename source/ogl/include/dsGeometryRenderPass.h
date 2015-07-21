#ifndef DS_GEOMETRYRENDERPASS_H
#define DS_GEOMETRYRENDERPASS_H

#include "dsRenderPass.h"

namespace ds {

class Scene;
class ObjectSelector;
class FrameBuffer;
class ShaderProgram;
class OpenGlRenderer;

class GeometryRenderPass : public RenderPass
{
public:

    GeometryRenderPass(Scene *pScene, OpenGlRenderer *pRenderer);
    ~GeometryRenderPass();

    const unsigned& colorTexture() const { return m_colorTexture; }
    const unsigned& positionTexture() const { return m_positionTexture; }
    const unsigned& normalTexture() const { return m_normalTexture; }
    const unsigned& depthTexture() const { return m_depthTexture; }

    void initialize();
    void perform();

private:

    void initializeFrameBuffer();

    Scene *m_pScene;
    OpenGlRenderer *m_pRenderer;
    ObjectSelector *m_pSelector;
    ShaderProgram *m_pShaderProgram;

    unsigned m_fbo;
    unsigned m_colorTexture;
    unsigned m_positionTexture;
    unsigned m_normalTexture;
    unsigned m_depthTexture;

};

} // namespace ds

#endif // DS_GEOMETRYRENDERPASS_H
