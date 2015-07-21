#include <SDL.h>
#include "dsLog.h"
#include "dsRenderer.h"
#include "dsEventRouter.h"
#include "dsQuitEvent.h"
#include "dsEngine.h"

namespace ds {

Engine* Engine::s_pEngine = nullptr;

Engine* Engine::instance()
{
    if (s_pEngine == nullptr) {
        s_pEngine = new Engine();
    }
    return s_pEngine;
}

void Engine::destroy()
{
    delete s_pEngine;
    s_pEngine = nullptr;
}

Engine::Engine()
    : EventHandler(),
      m_initialized(false),
      m_pSdlWindow(nullptr),
      m_pSdlRenderer(nullptr),
      m_pSdlGlContext(nullptr),
      m_pRenderer(nullptr),
      m_pEventRouter(new EventRouter()),
      m_running(false),
      m_fixedFpsDelay(0)
{
    // Register events
    registerEventFunc(this, &Engine::onQuitEvent);

    m_pEventRouter->addEventHandler(this);
}

Engine::~Engine()
{
    delete m_pRenderer;
    delete m_pEventRouter;
    SDL_GL_DeleteContext(m_pSdlGlContext);

    if (m_pSdlRenderer != nullptr) {
        SDL_DestroyRenderer(m_pSdlRenderer);
    }

    if (m_pSdlWindow != nullptr) {
        SDL_DestroyWindow(m_pSdlWindow);
    }

    dsInfo() << "Engine shut down.";
}

void Engine::setRenderer(Renderer *pRenderer)
{
    DS_ASSERT(pRenderer != nullptr);
    if (m_initialized) {
        dsError() << "Engine has been already initialized. Unable to assign the renderer.";
        return;
    }

    m_pRenderer = pRenderer;
}

bool Engine::initialize(const std::string &title, int width, int height, bool fullScreen)
{
    if (m_initialized) {
        dsError() << "Engine has been already initialized.";
        return false;
    }

    if (m_pRenderer == nullptr) {
        dsError() << "Unable initialize engine: no renderer has been set.";
        return false;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_TIMER |
                 SDL_INIT_AUDIO |
                 SDL_INIT_VIDEO |
                 SDL_INIT_JOYSTICK) < 0) {
        dsError() << "SDL initialization failed: " << SDL_GetError();
        return false;
    }

    int wFlags = SDL_WINDOW_OPENGL;
    if (fullScreen) {
        wFlags |= SDL_WINDOW_FULLSCREEN;
    }

    // Request OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Initialize window
    m_pSdlWindow = SDL_CreateWindow(title.c_str(),
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    width, height,
                                    wFlags);
    if (nullptr == m_pSdlWindow) {
        dsError() << "Unable to create SDL window: " << SDL_GetError();
        return false;
    }

    // Initialize SDL renderer
    m_pSdlRenderer = SDL_GetRenderer(m_pSdlWindow);
    if (nullptr == m_pSdlRenderer) {
        m_pSdlRenderer = SDL_CreateRenderer(m_pSdlWindow, -1, SDL_RENDERER_ACCELERATED |
                                                              SDL_RENDERER_PRESENTVSYNC);
    }

    if (nullptr == m_pSdlRenderer) {
        dsError() << "Unable to initialize SDL renderer: " << SDL_GetError();
        return false;
    }

    // Create OpenGL context
    m_pSdlGlContext = SDL_GL_CreateContext(m_pSdlWindow);
    if (nullptr == m_pSdlGlContext) {
        dsError() << "Unable to create OpenGL context: " << SDL_GetError();
    }

    // Initialize engine renderer
    int wWidth, wHeight;
    SDL_GetWindowSize(m_pSdlWindow, &wWidth, &wHeight);
    m_pRenderer->initialize(wWidth, wHeight);

    // Initialized OK.
    m_initialized = true;
    return true;
}

void Engine::setFixedFps(int fps)
{
    if (fps > 0) {
        m_fixedFpsDelay = 1000 / fps;
    } else {
        m_fixedFpsDelay = 0;
    }
}

void Engine::exec()
{
    if (!isInitialized()) {
        dsError() << "Engine has not been initialized";
        return;
    }

    DS_ASSERT(m_pRenderer != nullptr);

    dsInfo() << "Starting the engine loop";
    m_running = true;
    int prevFrameStart = SDL_GetTicks();
    while (m_running) {
        int frameStart = SDL_GetTicks();

        // Perform global update first
        int delta = frameStart - prevFrameStart;
        if (delta > 0) {
            // Don't send update event if delta time is zero
            update(delta);
        }

        // Dispatch all other events
        m_pEventRouter->collectEndDispatchEvents();

        // Perform rendering
        m_pRenderer->render();
        SDL_GL_SwapWindow(m_pSdlWindow);

        // Introduce delay for fixed FPS
        int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < m_fixedFpsDelay) {
            SDL_Delay(m_fixedFpsDelay - frameTime);
        }
        prevFrameStart = frameStart;
    }
    dsInfo() << "Engine loop is over";
}

void Engine::quit()
{
    dsInfo() << "Stopping the engine";
    m_running = false;
}

void Engine::registerUpdatable(IUpdatable *pUpdatable)
{
    DS_ASSERT(pUpdatable != nullptr);
    m_updatables.push_back(pUpdatable);
}

void Engine::update(int deltaMs)
{
    for (std::vector<IUpdatable*>::iterator it = m_updatables.begin();
         it != m_updatables.end();
         ++it) {
        (*it)->update(deltaMs);
    }
}

void Engine::onQuitEvent(QuitEvent *pEvent)
{
    DS_UNUSED(pEvent);
    quit();
}

} // namespace ds
