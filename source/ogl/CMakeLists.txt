project(ogl)

set(DEPENDS
    glm
    core
)

set(USE_SDL TRUE)
set(USE_SDL_IMAGE TRUE)
set(USE_OPENGL TRUE)
set(USE_GLEW TRUE)

include(build_library)
