#include <glew.h>
#include "dsShaderProgram.h"
#include "dsSpotLightWithShadow.h"

namespace ds {

const int cShadowMapWidth = 512;
const int cShadowMapHeight = 512;

const std::string vertexShaderSrc(
        "#version 330\n"
        "layout (location=0) in vec3 VertexPosition;\n"

        "uniform mat4 ProjectionViewMatrix;\n"
        "uniform mat4 ModelMatrix;\n"

        "out vec4 worldPosition;\n"

        "void main(void) {\n"
        "    worldPosition = ModelMatrix * vec4(VertexPosition, 1.0);\n"
        "    mat4 MVP = ProjectionViewMatrix * ModelMatrix;\n"
        "    gl_Position = MVP * vec4(VertexPosition, 1.0);\n"
        "}\n"
    );

const std::string fragmentShaderSrc(
        "#version 330\n"

        "layout (location=0) out float fDistance;\n"
        "in vec4 worldPosition;\n"
        "uniform vec3 LightPosition;\n"

        "void main(void) {\n"
        "    fDistance = distance(worldPosition.xyz, LightPosition);\n"
        "}\n"
    );

SpotLightWithShadow::SpotLightWithShadow()
    : Object(Type_SpotLightShadow),
      m_cutOffAngle(DS_DEG(50.0f)),
      m_color(1.0f, 1.0f, 1.0f),
      m_initialized(false)
{
}

bool SpotLightWithShadow::selectForRendering(Renderer *pRenderer)
{
    RenderPass::Id id = pRenderer->currentRenderPass()->id();
    return id == RenderPass::Id_SpotLightShadow ||
            id == RenderPass::Id_SpotLightShadowMap;
}

void SpotLightWithShadow::generateShadowMap()
{
    if (!initialize()) {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, cShadowMapWidth, cShadowMapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    // Render scene from lights poiint of view
    glm::mat4 projectionMatrix = glm::perspective(m_cutOffAngle, float(cShadowMapWidth)/float(cShadowMapHeight), 1.0f, 25.0f);
    glm::mat4 projectionViewMatrix = projectionMatrix * invertedWorldMatrix();

    (*m_pShaderProgram)["LightPosition"] = worldMatrix()[3].xyz();
    (*m_pShaderProgram)["ProjectionViewMatrix"] = projectionViewMatrix;
    // For all shadow caster, render it
    {
        //(*m_pShaderProgram)["ModelMatrix"] = pObject->worldMatrix();
        //pObject->render(pRenderer);
    }

    glCullFace(GL_BACK);
}

bool SpotLightWithShadow::initialize()
{
    if (m_initialized) {
        return true;
    }

    m_pShaderProgram = ShaderProgram::createFromSource(vertexShaderSrc, fragmentShaderSrc);
    DS_CHECK_GL_ERROR;
    if (m_pShaderProgram == nullptr) {
        return false;
    }

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    GLfloat border[4] = {1.0f, 0.0f, 0.0f, 0.0f };

    // Depth texture
    glGenTextures(1, &m_depthTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, cShadowMapWidth, cShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    // Verify frame buffer completeness
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        dsError() << "Frame buffer is incomplete, status=" << status;
    }

    DS_CHECK_GL_ERROR;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_initialized = true;
    return true;
}


} // namespace ds
