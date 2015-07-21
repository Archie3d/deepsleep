#ifndef DS_SELECTORRENDERPASS_H
#define DS_SELECTORRENDERPASS_H

#include "dsRenderPass.h"
#include "dsObject.h"

namespace ds {

class ObjectSelector;

class SelectorRenderPass : public RenderPass
{
public:

    SelectorRenderPass(Scene *pScene, Renderer *pRenderer);
    ~SelectorRenderPass();

    void setObjectType(Object::Type type) { m_objectType = type; }

    ObjectSelector* objectSelector() const { return m_pSelector; }

    void initialize();
    void perform();

private:

    Object::Type m_objectType;
    ObjectSelector *m_pSelector;
};

} // namespace ds


#endif // DS_SELECTORRENDERPASS_H
