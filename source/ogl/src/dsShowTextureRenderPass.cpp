#include <glew.h>
#include "dsGlobal.h"
#include "dsFrameBuffer.h"
#include "dsShaderProgram.h"
#include "dsGeometryRenderPass.h"
#include "dsShowTextureRenderPass.h"

namespace ds {

const std::string vertexShaderSrc(
        "#version 330\n"
        "layout (location=0) in vec2 PointCoord;\n"
        "layout (location=1) in vec2 TextureCoord;\n"
        "out vec2 texCoord;\n"
        "void main(void) {\n"
        "    gl_Position = vec4(PointCoord, 0.0, 1.0);\n"
        "    texCoord = TextureCoord;\n"
        "}"
    );
const std::string fragmentShaderSrc(
        "#version 330\n"
        "uniform sampler2D textureToDisplay;\n"
        "in vec2 texCoord;\n"
        "layout (location=0) out vec4 pixelColor;\n"
        "void main(void) {\n"
        "    pixelColor = texture(textureToDisplay, texCoord);\n"
        "}"
    );

ShowTextureRenderPass::ShowTextureRenderPass(GeometryRenderPass *pGeometryPass, OpenGlRenderer *pRenderer)
    : RenderPass(Id_Texture),
      m_pGeometryRenderPass(pGeometryPass),
      m_pRenderer(pRenderer),
      m_textures()
{
    DS_ASSERT(pRenderer != nullptr);
    m_pShaderProgram = nullptr;
    m_blengingEnabled = false;
    m_initialized = false;
}

ShowTextureRenderPass::~ShowTextureRenderPass()
{
    if (m_initialized) {
        glDeleteBuffers(1, &m_elementsBuffer);
        glDeleteBuffers(1, &m_texCoordBuffer);
        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteVertexArrays(1, &m_vertexArray);

        delete m_pShaderProgram;
    }
}

void ShowTextureRenderPass::initialize()
{
    if (m_initialized) {
        dsWarning() << "Render pass has been already initialized.";
        return;
    }

    m_pShaderProgram = ShaderProgram::createFromSource(vertexShaderSrc, fragmentShaderSrc);
    if (m_pShaderProgram == nullptr) {
        return;
    }

    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    // Points define the view port (where the drawing will be performed).
    GLfloat pointCoords[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f
    };

    // Coordinatex of two triangles vertices to coved the view port
    GLfloat texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    // Indices in texCoords array for two trianges covering the view port
    GLuint elements[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), pointCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_elementsBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementsBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), elements, GL_STATIC_DRAW);

    DS_CHECK_GL_ERROR;

    glBindVertexArray(0);

    m_initialized = true;
}

void ShowTextureRenderPass::perform()
{
    if (!m_initialized) {
        return;
    }

    m_pShaderProgram->use();

    // Enable blending with content already deiplayed
    if (m_blengingEnabled) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
    }

    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, *m_pTextureHandle);
    (*m_pShaderProgram)["textureToDisplay"] = 0;

    // Be sure to disable depth test
    glDisable(GL_DEPTH_TEST);

    // Draw the viewport
    glBindVertexArray(m_vertexArray);

    for (std::vector<const unsigned*>::const_iterator it = m_textures.begin();
         it != m_textures.end();
         ++it) {
        const unsigned *pTexture = *it;
        glBindTexture(GL_TEXTURE_2D, *pTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    DS_CHECK_GL_ERROR;

    // Clean-up
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    ShaderProgram::release();
    glDisable(GL_BLEND);

    DS_CHECK_GL_ERROR;
}

} // namespace ds
