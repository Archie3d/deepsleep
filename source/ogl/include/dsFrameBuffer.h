#ifndef DS_FRAMEBUFFER_H
#define DS_FRAMEBUFFER_H

#include "dsGlobal.h"

namespace ds {

/**
 * Frame buffer is used for rendering into four textures:
 * - color
 * - position (in world space)
 * - normal
 * - depth
 * These three buffers can be used further for shading.
 */
class FrameBuffer
{
public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    int width() const { return m_width; }
    int height() const { return m_height; }

    /**
     * Enable rendering to this frame buffer.
     * This will also clear the buffer.
     */
    void enable();

    /**
     * Disable rendering to this frame buffer.
     */
    void disable();

    unsigned colorTextureHandle() const { return m_colorTexture; }
    unsigned positionTextureHandle() const { return m_positionTexture; }
    unsigned normalTextureHandle() const { return m_normalTexture; }
    unsigned depthTextureHandle() const { return m_depthTexture; }

private:
    int m_width;
    int m_height;

    unsigned m_fbo;
    unsigned m_colorTexture;
    unsigned m_positionTexture;
    unsigned m_normalTexture;
    unsigned m_depthTexture;
};

} // namespace ds

#endif // DS_FRAMEBUFFER_H
