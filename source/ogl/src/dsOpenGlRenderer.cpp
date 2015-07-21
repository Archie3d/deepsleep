#include <glew.h>
#include <SDL.h>
#include "dsGlobal.h"
#include "dsOpenGlRenderer.h"

namespace ds {

OpenGlRenderer::OpenGlRenderer()
    : Renderer()
{
}

void OpenGlRenderer::initialize(int width, int height)
{
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    GLenum res = glewInit();
    if (res != 0) {
        dsError() << "glewInit returned " << res;
    }

    // Absorb GLEW error
    glGetError();

    int openGlMajorVersion;
    int openGlMinorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &openGlMajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &openGlMinorVersion);

    // Assert on OpenGL functions resolution
    DS_ASSERT(glGenFramebuffers);
    DS_ASSERT(glGenRenderbuffers);

    dsInfo() << "OpenGL " << openGlMajorVersion << "." << openGlMinorVersion << " detected.";

    Renderer::initialize(width, height);

    DS_CHECK_GL_ERROR;

    dsInfo() << "OpenGL renderer initialized";
}

void OpenGlRenderer::render()
{
    // Reset color and depth buffers for each rendering
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Renderer::render();
}

} // namespace ds
