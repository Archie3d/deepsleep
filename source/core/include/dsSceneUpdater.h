#ifndef DS_SCENEUPDATER_H
#define DS_SCENEUPDATER_H

#include "dsSceneNode.h"
#include "dsScene.h"
#include "dsIUpdatable.h"
#include "dsIVisitor.h"

namespace ds {

/**
 * This class dispatches the updates to the scenen updatable items.
 */
class SceneUpdater : public IUpdatable, public IVisitor<SceneNode>
{
public:

    SceneUpdater(Scene *pScene);

    // IUpdatable interface
    void update(int deltaMs);

    // IVisitor<SceneNode> interface
    void visit(SceneNode *pSceneNode);

private:
    Scene *m_pScene;
    mutable int m_delta;
};

} // namespace ds

#endif // DS_SCENEUPDATER_H
