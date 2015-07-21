#include "dsLog.h"
#include "dsGlobal.h"
#include "dsScene.h"
#include "dsSceneNode.h"

namespace ds {

SceneNode::SceneNode(Scene *pScene)
    : m_pScene(pScene),
      m_pParentNode(nullptr),
      m_pFirstChild(nullptr),
      m_pPrevSibling(nullptr),
      m_pNextSibling(nullptr),
      m_matrix(1.0f),
      m_worldMatrixValid(false),
      m_worldMatrix(1.0f),
      m_invertedWorldMatrix(1.0f)
{
}

SceneNode::~SceneNode()
{
    detach();

    // Delete children nodes
    SceneNode *pNode = m_pFirstChild;
    while (pNode != nullptr) {
        SceneNode *pTmp = pNode;
        pNode = pNode->m_pNextSibling;
        delete pTmp;
    }
}

void SceneNode::setTransformation(const glm::mat4x4 &m)
{
    m_matrix = m;
    invalidateWorldMatrix();
}

const glm::mat4x4& SceneNode::worldMatrix() const
{
    if (!m_worldMatrixValid) {
        computeWorldMatrix();
    }
    return m_worldMatrix;
}

const glm::mat4x4& SceneNode::invertedWorldMatrix() const
{
    if (!m_worldMatrixValid) {
        computeWorldMatrix();
    }
    return m_invertedWorldMatrix;
}

void SceneNode::translate(const glm::vec3 &v)
{
    m_matrix = glm::translate(m_matrix, v);
    invalidateWorldMatrix();
}

void SceneNode::rotate(const glm::vec3 &axis, float angle)
{
    m_matrix = glm::rotate(m_matrix, angle, axis);
    invalidateWorldMatrix();
}

void SceneNode::scale(const glm::vec3 &v)
{
    m_matrix = glm::scale(m_matrix, v);
    invalidateWorldMatrix();
}

void SceneNode::addChild(SceneNode *pNode)
{
    DS_ASSERT(pNode != nullptr);

    if (this == pNode->m_pParentNode) {
        // Already parented
        return;
    }

    if (pNode->m_pParentNode != nullptr) {
        pNode->m_pParentNode->removeChild(pNode);
    } else if (nullptr != pNode->m_pScene) {
        pNode->m_pScene->rootNode()->removeNode(pNode);
    }

    if (nullptr != m_pFirstChild) {
        m_pFirstChild->m_pPrevSibling = pNode;
        pNode->m_pNextSibling = m_pFirstChild;
    }
    m_pFirstChild = pNode;

    pNode->m_pParentNode = this;
    pNode->m_pScene = m_pScene;
    pNode->updateChildrenScene(m_pScene);

    // Invalidate node position because of the parenting change
    pNode->invalidateWorldMatrix();
}

void SceneNode::removeChild(SceneNode *pNode)
{
    DS_ASSERT(pNode != nullptr);

    if (this != pNode->m_pParentNode) {
        // This is not a direct child
        return;
    }
    pNode->detach();
}

bool SceneNode::removeNode(SceneNode *pNode)
{
    DS_ASSERT(pNode != nullptr);

    if (this == pNode->m_pParentNode) {
        // Direct child
        pNode->detach();
        return true;
    }

    SceneNode *pChildNode = m_pFirstChild;
    while (pChildNode != nullptr) {
        if (pChildNode->removeNode(pNode)) {
            return true;
        }
        pChildNode = pChildNode->m_pNextSibling;
    }
    return false;
}

void SceneNode::detach()
{
    if (m_pPrevSibling != nullptr) {
        m_pPrevSibling->m_pNextSibling = m_pNextSibling;
    }
    if (m_pNextSibling != nullptr) {
        m_pNextSibling->m_pPrevSibling = m_pPrevSibling;
    }

    SceneNode *pParentNode = m_pParentNode;
    if (pParentNode != nullptr) {
        if (this == pParentNode->m_pFirstChild) {
            pParentNode->m_pFirstChild = m_pNextSibling;
        }
    }

    m_pNextSibling = nullptr;
    m_pPrevSibling = nullptr;
    m_pParentNode = nullptr;
    m_pScene = nullptr;

    updateChildrenScene(nullptr);
}

void SceneNode::accept(IVisitor<SceneNode> *pVisitor)
{
    pVisitor->visit(this);
    SceneNode *pNode = m_pFirstChild;
    while (pNode != nullptr) {
        pNode->accept(pVisitor);
        pNode = pNode->m_pNextSibling;
    }
}

void SceneNode::updateChildrenScene(Scene *pScene)
{
    SceneNode *pNode = m_pFirstChild;
    while (nullptr != pNode) {
        pNode->m_pScene = pScene;
        pNode->updateChildrenScene(pScene);
        pNode = pNode->m_pNextSibling;
    }
}

void SceneNode::invalidateWorldMatrix() const
{
    if (!m_worldMatrixValid) {
        // Already invalid
        return;
    }

    m_worldMatrixValid = false;

    // Invalidate al chiildren recursively
    SceneNode *pNode = m_pFirstChild;
    while (nullptr != pNode) {
        pNode->invalidateWorldMatrix();
        pNode = pNode->m_pNextSibling;
    }
}

void SceneNode::computeWorldMatrix() const
{
    if (nullptr != m_pParentNode) {
        m_worldMatrix = m_pParentNode->worldMatrix() * m_matrix;
    }
    m_invertedWorldMatrix = glm::inverse(m_worldMatrix);
    m_worldMatrixValid = true;
}

} // namespace ds
