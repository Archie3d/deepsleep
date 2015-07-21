#include <glew.h>
#include "dsGlobal.h"
#include "dsScene.h"
#include "dsGeometryRenderPass.h"
#include "dsFrameBuffer.h"
#include "dsOpenGlRenderer.h"
#include "dsShaderProgram.h"
#include "dsAmbientLightingRenderPass.h"

namespace ds {

const std::string vertexShaderSrc(
        "#version 330\n"
        "layout (location=0) in vec2 PointCoordinates;\n"
        "layout (location=1) in vec2 TextureCoordinates;\n"
        "out vec2 texCoord;\n"
        "void main(void) {\n"
        "    gl_Position = vec4(PointCoordinates, 0.0, 1.0);\n"
        "    texCoord = TextureCoordinates;\n"
        "}"
);

const std::string fragmentShaderSrc(
        "#version 330\n"
        "#define M_PI 3.1415926535897932384626433832795\n"
        "uniform vec3 LightColor;\n"
        "uniform sampler2D ColorTexture;\n"
        "in vec2 texCoord;\n"
        "layout (location=0) out vec4 pixelColor;\n"
        "void main(void) {\n"
        "    vec4 color = texture(ColorTexture, texCoord);\n"
        "    pixelColor = vec4(LightColor, 1.0) * color;\n"
        "}"
    );

AmbientLightingRenderPass::AmbientLightingRenderPass(Scene *pScene,
                                                     OpenGlRenderer *pRenderer)
    : RenderPass(Id_AmbientLight),
      m_pScene(pScene),
      m_pRenderer(pRenderer),
      m_pColorTexture(nullptr)
{
    DS_ASSERT(pScene != nullptr);
    DS_ASSERT(pRenderer != nullptr);
    m_pShaderProgram = nullptr;
    m_initialized = false;
}

AmbientLightingRenderPass::~AmbientLightingRenderPass()
{
    if (m_initialized) {
        glDeleteBuffers(1, &m_elementsBuffer);
        glDeleteBuffers(1, &m_texCoordBuffer);
        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteVertexArrays(1, &m_vertexArray);

        glDeleteTextures(1, &m_colorTexture);
        glDeleteBuffers(1, &m_depthBuffer);
        glDeleteFramebuffers(1, &m_fbo);
    }
    delete m_pShaderProgram;
}

void AmbientLightingRenderPass::initialize()
{
    if (m_initialized) {
        dsWarning() << "Render pass has been already initialized";
        return;
    }

    m_pShaderProgram = ShaderProgram::createFromSource(vertexShaderSrc, fragmentShaderSrc);
    if (!m_pShaderProgram) {
        return;
    }

    initializeFbo();

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

void AmbientLightingRenderPass::perform()
{
    if (!m_initialized) {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_pRenderer->width(), m_pRenderer->height());

    // clear frame buffer
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum buffers[] = {
        GL_COLOR_ATTACHMENT0
    };
    glDrawBuffers(1, buffers);

    m_pShaderProgram->use();
    (*m_pShaderProgram)["LightColor"] = m_pScene->ambientLightColor();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *m_pColorTexture);
    (*m_pShaderProgram)["ColorTexture"] = 0;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    DS_CHECK_GL_ERROR

    glBindVertexArray(m_vertexArray);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDisable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AmbientLightingRenderPass::initializeFbo()
{
    DS_ASSERT(m_pRenderer != nullptr);
    int width = m_pRenderer->width();
    int height = m_pRenderer->height();

    glGenFramebuffers(1, &m_fbo);
    glGenRenderbuffers(1, &m_depthBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Color texture
    glGenTextures(1, &m_colorTexture);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

    // Verify frame buffer completeness
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        dsError() << "Frame buffer is incomplete, status=" << status;
    }

    DS_CHECK_GL_ERROR;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace ds
