#ifndef DS_SCENENODE_H
#define DS_SCENENODE_H

#include "dsGlobal.h"
#include "dsMath.h"
#include "dsIVisitor.h"

namespace ds {

class Scene;

/**
 * Node existing in a scene.
 * All nodes are organized in hierarchical manner. A node
 * can have several child nodes, but only one parent.
 * All nodes should be put on a scene.
 */
class SceneNode
{
    friend class Scene;

public:

    SceneNode(Scene *pScene = nullptr);
    virtual ~SceneNode();

    Scene* scene() const { return m_pScene; }
    const glm::mat4x4& transformation() const { return m_matrix; }
    void setTransformation(const glm::mat4x4 &m);
    const glm::mat4x4& worldMatrix() const;
    const glm::mat4x4& invertedWorldMatrix() const;

    /**
     * Translate this node.
     * @param v Translation vector.
     */
    void translate(const glm::vec3 &v);

    /**
     * Rotate this node.
     * @param axis Rotation axis.
     * @param angle Rotation angle.
     */
    void rotate(const glm::vec3 &axis, float angle);

    /**
     * Scale the object
     * @param v scaling factors for each axis.
     */
    void scale(const glm::vec3 &v);

    void addChild(SceneNode *pNode);
    void removeChild(SceneNode *pNode);
    bool removeNode(SceneNode *pNode);
    void detach();

    /**
     * Tells whether the node is a null node.
     * Null nodes are not directly associated to any objects and are
     * used only as a part of transformation chain.
     * @return
     */
    virtual bool isNull() const { return true; }

    SceneNode* firstChild() const { return m_pFirstChild; }
    SceneNode* nextSibling() const { return m_pNextSibling; }
    SceneNode *prevSibling() const { return m_pPrevSibling; }
    SceneNode* parentNode() const { return m_pParentNode; }

    /**
     * Accept a visitor object on this node and
     * on all children nodes recursively.
     * @param pVisitor
     */
    void accept(IVisitor<SceneNode> *pVisitor);

protected:

    /**
     * Update all children of this node recursively with corresponding scene.
     * @param pScene
     */
    void updateChildrenScene(Scene *pScene);

    /**
     * Invalidate the world matrix cache forcing the world
     * matrix to be recomputed when queried next time.
     */
    void invalidateWorldMatrix() const;

    /**
     * Compute world matrix and its inverse.
     */
    void computeWorldMatrix() const;

private:

    DS_DISABLE_COPY(SceneNode)

    /// Pointer to scene that holds this node.
    Scene *m_pScene;

    SceneNode *m_pParentNode;   ///< Parent node.
    SceneNode *m_pFirstChild;   ///< First child node.
    SceneNode *m_pPrevSibling;  ///< Previous node in the linked list.
    SceneNode *m_pNextSibling;  ///< Next node in the linked list.

    /// Local transformation (relative to parent).
    glm::mat4x4 m_matrix;

    /// World matrix cache.
    mutable bool m_worldMatrixValid;
    mutable glm::mat4x4 m_worldMatrix;
    mutable glm::mat4x4 m_invertedWorldMatrix;
};

} // namespace ds

#endif // DS_SCENENODE_H
