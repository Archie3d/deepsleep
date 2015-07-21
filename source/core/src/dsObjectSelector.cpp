#include "dsObjectSelector.h"

namespace ds {

ObjectSelector::ObjectSelector(Scene *pScene, Renderer *pRenderer)
    : m_pScene(pScene),
      m_pRenderer(pRenderer),
      m_objectType(Object::Type_Null),
      m_selectedObjects()
{
}

void ObjectSelector::select(Object::Type type)
{
    m_objectType = type;
    m_selectedObjects.clear();
    m_pScene->rootNode()->accept(this);
}

void ObjectSelector::clear()
{
    m_selectedObjects.clear();
}

void ObjectSelector::visit(SceneNode *pSceneNode)
{
    Object *pObject = dynamic_cast<Object*>(pSceneNode);
    if (pObject != nullptr && pObject->type() == m_objectType) {
        if (pObject->selectForRendering(m_pRenderer)) {
            m_selectedObjects.push_back(pObject);
        }
    }
}

} // namespace ds
