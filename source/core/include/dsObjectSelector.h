#ifndef DS_OBJECTSELECTOR_H
#define DS_OBJECTSELECTOR_H

#include <vector>
#include "dsObject.h"
#include "dsRenderer.h"
#include "dsIVisitor.h"
#include "dsSceneNode.h"
#include "dsScene.h"

namespace ds {

/**
 * This class performs selection of renderable objects.
 */
class ObjectSelector : public IVisitor<SceneNode>
{
public:

    ObjectSelector(Scene *pScene, Renderer *pRenderer);

    const std::vector<Object*>& selectedObjects() const { return m_selectedObjects; }

    void select(Object::Type type);
    void clear();

    void visit(SceneNode *pSceneNode);

private:
    Scene *m_pScene;
    Renderer *m_pRenderer;
    Object::Type m_objectType;  ///< Object type to select.
    std::vector<Object*> m_selectedObjects;
};

} // namespace ds

#endif // DS_RENDERABLESELECTOR_H
