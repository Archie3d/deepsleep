#include <glew.h>
#include "dsMesh.h"
#include "dsMeshObject.h"

namespace ds {

MeshObject::MeshObject(const ResourceHandle &mesh)
    : Object(Type_Geometry),
      m_meshResourceHandle(mesh),
      m_initialized(false),
      m_indexBufferHandle(0),
      m_vertexBufferHandle(0),
      m_normalBufferHandle(0),
      m_vertexArrayHandle(0)
{
}

MeshObject::~MeshObject()
{
    if (m_initialized) {
        glDeleteBuffers(1, &m_indexBufferHandle);
        glDeleteBuffers(1, &m_vertexBufferHandle);
        glDeleteBuffers(1, &m_normalBufferHandle);
        glDeleteVertexArrays(1, &m_vertexArrayHandle);
    }
}

bool MeshObject::selectForRendering(Renderer *pRenderer)
{
    return initialize() &&
            pRenderer->currentRenderPass()->id() == RenderPass::Id_Geometry;
}

void MeshObject::render(Renderer *pRenderer)
{
    DS_UNUSED(pRenderer);
    glBindVertexArray(m_vertexArrayHandle);
    glDrawElements(GL_TRIANGLES, m_nFaces, GL_UNSIGNED_INT, 0);
}

bool MeshObject::initialize()
{
    if (m_initialized) {
        return true;
    }

    if (m_meshResourceHandle.isNull()) {
        return false;
    }

    Mesh *pMesh = m_meshResourceHandle.to<Mesh>();
    const std::vector<glm::vec3> &vertices = pMesh->vertices();
    const std::vector<glm::vec3> &normals = pMesh->normals();
    const std::vector<int> &faces = pMesh->faces();
    m_nFaces = static_cast<int>(faces.size());

    glGenVertexArrays(1, &m_vertexArrayHandle);
    glBindVertexArray(m_vertexArrayHandle);

    GLfloat *pVertices = new GLfloat[3 * vertices.size()];
    {
        int index = 0;
        for(std::vector<glm::vec3>::const_iterator it = vertices.begin();
            it != vertices.end();
            ++it) {
            glm::vec3 v = *it;
            pVertices[index++] = v.x;
            pVertices[index++] = v.y;
            pVertices[index++] = v.z;
        }
    }

    glGenBuffers(1, &m_vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(GLfloat), pVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    delete[] pVertices;

    GLfloat *pNormals = new GLfloat[3 * normals.size()];
    {
        int index = 0;
        for(std::vector<glm::vec3>::const_iterator it = normals.begin();
            it != normals.end();
            ++it) {
            glm::vec3 v = *it;
            pNormals[index++] = v.x;
            pNormals[index++] = v.y;
            pNormals[index++] = v.z;
        }
    }

    glGenBuffers(1, &m_normalBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(GLfloat), pNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    delete[] pNormals;

    GLuint *pElements = new GLuint[faces.size()];
    {
        int index = 0;
        for(std::vector<int>::const_iterator it = faces.begin();
            it != faces.end();
            ++it) {
            pElements[index++] = static_cast<GLuint>(*it);
        }
    }

    glGenBuffers(1, &m_indexBufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint), pElements, GL_STATIC_DRAW);
    delete[] pElements;

    DS_CHECK_GL_ERROR

    glBindVertexArray(0);

    m_initialized = true;
    return true;
}

} // namespace ds
