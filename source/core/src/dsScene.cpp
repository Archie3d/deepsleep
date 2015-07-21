#include "dsGlobal.h"
#include "dsSceneNode.h"
#include "dsCamera.h"
#include "dsScene.h"

namespace ds {

Scene::Scene()
    : m_pRootNode(new SceneNode(this)),
      m_pActiveCamera(nullptr),
      m_ambientLight(0.0f, 0.0f, 0.0f)
{
}

Scene::~Scene()
{
    delete m_pRootNode;
}

void Scene::addNode(SceneNode *pNode)
{
    DS_ASSERT(pNode != nullptr);
    m_pRootNode->addChild(pNode);
}

void Scene::removeNode(SceneNode *pNode)
{
    DS_ASSERT(pNode != nullptr);
    m_pRootNode->removeNode(pNode);
}

} // namespace ds
