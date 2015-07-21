#include "dsSceneUpdater.h"

namespace ds {

SceneUpdater::SceneUpdater(Scene *pScene)
    : m_pScene(pScene)
{
    DS_ASSERT(pScene != nullptr);
}

void SceneUpdater::update(int deltaMs)
{
    m_delta = deltaMs;
    m_pScene->rootNode()->accept(this);
}

void SceneUpdater::visit(SceneNode *pSceneNode)
{
    IUpdatable *pUpdatable = dynamic_cast<IUpdatable*>(pSceneNode);
    if (pUpdatable != nullptr) {
        pUpdatable->update(m_delta);
    }
}

} // namespace ds
