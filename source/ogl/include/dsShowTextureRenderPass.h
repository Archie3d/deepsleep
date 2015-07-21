#ifndef DS_SHOWTEXTURERENDERPASS_H
#define DS_SHOWTEXTURERENDERPASS_H

#include <vector>
#include "dsRenderPass.h"

namespace ds {

class OpenGlRenderer;
class ShaderProgram;
class GeometryRenderPass;

class ShowTextureRenderPass : public RenderPass
{
public:

    ShowTextureRenderPass(GeometryRenderPass *pGeometryPass, OpenGlRenderer *pRenderer);
    ~ShowTextureRenderPass();

    /**
     * Assign a handle for texture to show.
     * @param handle
     */
    void addTexture(const unsigned *pHandle) { m_textures.push_back(pHandle); }
    void setBlending(bool v) { m_blengingEnabled = v; }

    void initialize();

    void perform();

private:
    GeometryRenderPass *m_pGeometryRenderPass;
    OpenGlRenderer *m_pRenderer;
    ShaderProgram *m_pShaderProgram;

    // Textures to be rendered
    std::vector<const unsigned*> m_textures;

    bool m_initialized;
    bool m_blengingEnabled;

    unsigned m_vertexArray;
    unsigned m_vertexBuffer;
    unsigned m_texCoordBuffer;
    unsigned m_elementsBuffer;
};

} // namespace ds

#endif // DS_SHOWTEXTURERENDERPASS_H
