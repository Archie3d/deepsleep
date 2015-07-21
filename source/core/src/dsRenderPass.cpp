#include "dsGlobal.h"
#include "dsRenderPass.h"

namespace ds {

RenderPass::RenderPass(Id id)
    : m_id(id),
      m_enabled(true)
{
}

RenderPass::~RenderPass()
{
}

} // namespace ds
