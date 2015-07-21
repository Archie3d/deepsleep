#ifndef DS_IRENDERABLE_H
#define DS_IRENDERABLE_H

#include "dsRenderer.h"

namespace ds {

/**
 * Interface to renderable objects.
 */
class IRenderable
{
public:

    virtual bool selectForRendering(Renderer *pRenderer) = 0;

    virtual void render(Renderer *pRenderer) = 0;
};

} // namespace ds

#endif // DS_IRENDERABLE_H
