#include "dsObjectSelector.h"
#include "dsSelectorRenderPass.h"

namespace ds {

SelectorRenderPass::SelectorRenderPass(Scene *pScene, Renderer *pRenderer)
    : RenderPass(Id_Selection),
      m_pSelector(new ObjectSelector(pScene, pRenderer)),
      m_objectType(Object::Type_Null)
{
}

SelectorRenderPass::~SelectorRenderPass()
{
    delete m_pSelector;
}

void SelectorRenderPass::initialize()
{
}

void SelectorRenderPass::perform()
{
    m_pSelector->select(m_objectType);
}

} // namespace ds
