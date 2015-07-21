#ifndef DS_OPENGLRENDERER_H
#define DS_OPENGLRENDERER_H

#include "dsRenderer.h"

namespace ds {

class OpenGlRenderer : public Renderer
{
public:
    OpenGlRenderer();

    void initialize(int width, int height);

    void render();
};

} // namespace ds

#endif // DS_OPENGLRENDERER_H
