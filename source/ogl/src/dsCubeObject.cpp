#include <glew.h>
#include "dsCubeObject.h"

namespace ds {

CubeObject::CubeObject()
    : Object(Type_Geometry),
      m_initialized(false)
{
}

CubeObject::~CubeObject()
{
    if (m_initialized) {
        glDeleteBuffers(1, &m_indexBufferHandle);
        glDeleteBuffers(1, &m_vertexBufferHandle);
        glDeleteBuffers(1, &m_normalBufferHandle);
        glDeleteVertexArrays(1, &m_vertexArrayHandle);
    }
}

bool CubeObject::selectForRendering(Renderer *pRenderer)
{
    // Select only if initialized Ok.
    return initialize() &&
            pRenderer->currentRenderPass()->id() == RenderPass::Id_Geometry;
}

void CubeObject::render(Renderer *pRenderer)
{
    DS_UNUSED(pRenderer);
    glBindVertexArray(m_vertexArrayHandle);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

bool CubeObject::initialize()
{
    if (m_initialized) {
        return true;
    }

    float side = 1.0f;
    float side2 = side / 2.0f;

    GLfloat vertices[] = {
        // Front
       -side2, -side2, side2,
        side2, -side2, side2,
        side2,  side2, side2,
       -side2,  side2, side2,
       // Right
        side2, -side2, side2,
        side2, -side2, -side2,
        side2,  side2, -side2,
        side2,  side2, side2,
       // Back
       -side2, -side2, -side2,
       -side2,  side2, -side2,
        side2,  side2, -side2,
        side2, -side2, -side2,
       // Left
       -side2, -side2, side2,
       -side2,  side2, side2,
       -side2,  side2, -side2,
       -side2, -side2, -side2,
       // Bottom
       -side2, -side2, side2,
       -side2, -side2, -side2,
        side2, -side2, -side2,
        side2, -side2, side2,
       // Top
       -side2,  side2, side2,
        side2,  side2, side2,
        side2,  side2, -side2,
       -side2,  side2, -side2
    };

    GLfloat normals[] = {
        // Front
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        // Right
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        // Back
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        // Left
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        // Bottom
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        // Top
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    GLuint elements[] = {
        0,1,2,0,2,3,
        4,5,6,4,6,7,
        8,9,10,8,10,11,
        12,13,14,12,14,15,
        16,17,18,16,18,19,
        20,21,22,20,22,23
    };

    glGenVertexArrays(1, &m_vertexArrayHandle);
    glBindVertexArray(m_vertexArrayHandle);

    glGenBuffers(1, &m_vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_normalBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_indexBufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), elements, GL_STATIC_DRAW);

    DS_CHECK_GL_ERROR

    glBindVertexArray(0);

    m_initialized = true;
    return true;
}

} // namespace ds
