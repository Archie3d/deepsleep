#ifndef DS_SCENE_H
#define DS_SCENE_H

#include "dsMath.h"

namespace ds {

class SceneNode;
class Camera;

/**
 * Base class to handle scene composed of nodes.
 */
class Scene {
public:

    Scene();
    ~Scene();

    SceneNode* rootNode() const { return m_pRootNode; }
    void addNode(SceneNode *pNode);
    void removeNode(SceneNode *pNode);

    Camera* activeCamera() const { return m_pActiveCamera; }
    void setActiveCamera(Camera *pCamera) { m_pActiveCamera = pCamera; }

    glm::vec3 ambientLightColor() const { return m_ambientLight; }
    void setAmbientLightColor(const glm::vec3 &c) { m_ambientLight = c; }

private:
    SceneNode *m_pRootNode;     ///< Root node.
    Camera *m_pActiveCamera;    ///< Active camera.
    glm::vec3 m_ambientLight;   ///< Scene ambient light.
};

} // namespace ds

#endif // DS_SCENE_H
