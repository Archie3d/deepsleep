#ifndef DS_RENDERER_H
#define DS_RENDERER_H

#include <vector>
#include "dsRenderPass.h"

namespace ds {

/**
 * Abstract renderer.
 */
class Renderer
{
public:

    /**
     * Construct a renderer by do not initialize it.
     */
    Renderer();

    /**
     * Destructor.
     * This will also delete all render passes.
     */
    virtual ~Renderer();

    /**
     * Initialize renderer with a given viewport size.
     * This will also initialize all render passes.
     * @param width Viewport width.
     * @param height Viewport height.
     */
    virtual void initialize(int width, int height);

    int width() const { return m_width; }
    int height() const { return m_height; }

    /**
     * Returns current render pass being rendered.
     * @return
     */
    RenderPass* currentRenderPass() const { return m_pCurrentRenderPass; }

    /**
     * Add a render pass at the end of existing rendering chain.
     * Render pass object will be acquired and deleted along with
     * the renderer.
     * @param pRenderPass Pointer to render pass to be added.
     */
    void addRenderPass(RenderPass *pRenderPass);

    /**
     * Perform rendering by executing all rendering passes in sequence.
     */
    virtual void render();

protected:

    /**
     * Initialize all render passes in its order.
     */
    void initializeRenderPasses();

    /**
     * Perform a rendering pass.
     * @param pRenderPass
     */
    void performRenderPass(RenderPass *pRenderPass);

private:

    int m_width;    ///< Viewport width (in pixels).
    int m_height;   ///< Viewport height (in pixels).

    /// List of render passes.
    std::vector<RenderPass*> m_renderPasses;

    /// Render pass currently being rendered.
    RenderPass *m_pCurrentRenderPass;
};

} // namespace ds

#endif // DS_RENDERER_H
