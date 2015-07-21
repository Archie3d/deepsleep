#include <glew.h>
#include "dsCamera.h"
#include "dsOpenGlRenderer.h"
#include "dsShaderProgram.h"
#include "dsObjectSelector.h"
#include "dsGeometryRenderPass.h"

namespace ds {

const std::string vertexShaderSrc(
        "#version 330\n"
        "layout (location=0) in vec3 VertexPosition;\n"
        "layout (location=1) in vec3 VertexNormal;\n"

        "uniform mat4 ProjectionViewMatrix;\n"
        "uniform mat4 ModelMatrix;\n"

        "out vec4 position;\n"
        "out vec3 normal;\n"

        "void main(void) {\n"
        "    mat4 MVP = ProjectionViewMatrix * ModelMatrix;\n"
        "    gl_Position = MVP * vec4(VertexPosition, 1.0);\n"
        "    position = ModelMatrix * vec4(VertexPosition, 1.0);\n"
        "    normal = mat3(ModelMatrix) * VertexNormal;\n"
        "}\n"
    );

const std::string fragmentShaderSrc(
        "#version 330\n"

        "in vec4 position;\n"
        "in vec3 normal;\n"

        "layout (location=0) out vec4 outColor;\n"
        "layout (location=1) out vec4 outPosition;\n"
        "layout (location=2) out vec3 outNormal;\n"

        "void main(void) {\n"
        "    outColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "    outPosition = position;\n"
        "    outNormal = normal;\n"
        "}\n"
    );

GeometryRenderPass::GeometryRenderPass(Scene *pScene, OpenGlRenderer *pRenderer)
    : RenderPass(Id_Geometry),
      m_pScene(pScene),
      m_pRenderer(pRenderer),
      m_pSelector(nullptr),
      m_pShaderProgram(nullptr),
      m_fbo(0),
      m_colorTexture(0),
      m_positionTexture(0),
      m_normalTexture(0),
      m_depthTexture(0)
{
    DS_ASSERT(m_pRenderer != nullptr);
    m_pSelector = new ObjectSelector(pScene, pRenderer);
}

GeometryRenderPass::~GeometryRenderPass()
{
    delete m_pSelector;
    delete m_pShaderProgram;

    glDeleteTextures(1, &m_colorTexture);
    glDeleteTextures(1, &m_positionTexture);
    glDeleteTextures(1, &m_normalTexture);
    glDeleteTextures(1, &m_depthTexture);

    glDeleteFramebuffers(1, &m_fbo);
}

void GeometryRenderPass::initialize()
{
    initializeFrameBuffer();
    m_pShaderProgram = ShaderProgram::createFromSource(vertexShaderSrc, fragmentShaderSrc);
    DS_CHECK_GL_ERROR;
}

void GeometryRenderPass::perform()
{
    if (m_pShaderProgram == nullptr) {
        return;
    }

    Camera *pCamera = m_pScene->activeCamera();
    if (pCamera == nullptr) {
        // No active camera has been set
        return;
    }

    m_pSelector->select(Object::Type_Geometry);

    m_pShaderProgram->use();
    (*m_pShaderProgram)["ProjectionViewMatrix"] = pCamera->projectionViewMatrix();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_pRenderer->width(), m_pRenderer->height());
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    glDrawBuffers(3, buffers);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Render all selected objects
    const std::vector<Object*> &selection = m_pSelector->selectedObjects();
    std::vector<Object*>::const_iterator it = selection.begin();
    while (it != selection.end()) {
        Object *pObject = *it;
        (*m_pShaderProgram)["ModelMatrix"] = pObject->worldMatrix();
        DS_CHECK_GL_ERROR
        pObject->render(m_pRenderer);
        ++it;
    }

    //glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ShaderProgram::release();

    DS_CHECK_GL_ERROR;
}

void GeometryRenderPass::initializeFrameBuffer()
{
    DS_ASSERT(m_pRenderer != nullptr);
    int width = m_pRenderer->width();
    int height = m_pRenderer->height();

    glGenFramebuffers(1, &m_fbo);
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

    // Position texture
    glGenTextures(1, &m_positionTexture);
    glBindTexture(GL_TEXTURE_2D, m_positionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_positionTexture, 0);

    // Normal texture
    glGenTextures(1, &m_normalTexture);
    glBindTexture(GL_TEXTURE_2D, m_normalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_normalTexture, 0);

    // Depth texture
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    // Verify frame buffer completeness
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        dsError() << "Frame buffer is incomplete, status=" << status;
    }

    DS_CHECK_GL_ERROR;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

} // namespace ds
