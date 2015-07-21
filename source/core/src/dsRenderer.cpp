#include "dsGlobal.h"
#include "dsRenderer.h"

namespace ds {

Renderer::Renderer()
    : m_width(0),
      m_height(0),
      m_renderPasses(),
      m_pCurrentRenderPass(nullptr)
{
}

Renderer::~Renderer()
{
    std::vector<RenderPass*>::iterator it = m_renderPasses.begin();
    while (it != m_renderPasses.end()) {
        RenderPass *pPass = *it;
        delete pPass;
        ++it;
    }
}

void Renderer::initialize(int width, int height)
{
    m_width = width;
    m_height = height;
    initializeRenderPasses();
}

void Renderer::addRenderPass(RenderPass *pRenderPass)
{
    DS_ASSERT(pRenderPass != nullptr);
    m_renderPasses.push_back(pRenderPass);
}

void Renderer::initializeRenderPasses()
{
    for (std::vector<RenderPass*>::iterator it = m_renderPasses.begin();
         it != m_renderPasses.end();
         ++it) {
        (*it)->initialize();
    }
}

void Renderer::render()
{
    std::vector<RenderPass*>::iterator it = m_renderPasses.begin();
    while (it != m_renderPasses.end()) {
        performRenderPass(*it);
        ++it;
    }
    m_pCurrentRenderPass = nullptr;
}

void Renderer::performRenderPass(RenderPass *pRenderPass)
{
    DS_ASSERT(pRenderPass != nullptr);
    if (pRenderPass->isEnabled()) {
        m_pCurrentRenderPass = pRenderPass;
        pRenderPass->perform();
    }
}

} // namespace ds
