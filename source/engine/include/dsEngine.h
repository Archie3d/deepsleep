#ifndef DS_ENGINE_H
#define DS_ENGINE_H

#include <string>
#include <vector>
#include "dsGlobal.h"
#include "dsIUpdatable.h"
#include "dsEventRouter.h"

// Forward declaration to avoid SDL includes.
struct SDL_Window;
struct SDL_Renderer;
typedef void *SDL_GLContext;

namespace ds {

class Renderer;
class QuitEvent;

/**
 * Central class to control the engine.
 */
class Engine : public EventHandler
{
public:

    static Engine* instance();
    static void destroy();

    Renderer* renderer() const { m_pRenderer; }
    void setRenderer(Renderer *pRenderer);

    /**
     * Initialize the engine.
     * @param title Window title.
     * @param width View port width in pixels.
     * @param height View port height in pixels.
     * @param fullScreen true for full screen view.
     * @return
     */
    bool initialize(const std::string &title,
                    int width, int height,
                    bool fullScreen = false);

    /**
     * Tells whether the engine has been initialized.
     * @return
     */
    bool isInitialized() const { return m_initialized; }

    /**
     * Set fixed FPS value.
     * Zero value will set unlimited FPS.
     * @param fps
     */
    void setFixedFps(int fps);

    /**
     * Run the engine.
     * This function will block till the engine loop stops.
     */
    void exec();

    /**
     * Stop the engine loop.
     */
    void quit();

    /**
     * Register an object to be notified on engine updates.
     * @param pUpdatable
     */
    void registerUpdatable(IUpdatable *pUpdatable);

protected:

    /**
     * Handle engine periodic updates.
     * @param deltaMs Time passed since last update.
     */
    void update(int deltaMs);

    void onQuitEvent(QuitEvent *pEvent);

private:
    DS_DISABLE_COPY(Engine)

    static Engine *s_pEngine;

    Engine();
    ~Engine();

    bool m_initialized;

    SDL_Window *m_pSdlWindow;
    SDL_Renderer *m_pSdlRenderer;
    SDL_GLContext m_pSdlGlContext;

    Renderer *m_pRenderer;
    EventRouter *m_pEventRouter;

    ///< List of updatable objects.
    std::vector<IUpdatable*> m_updatables;

    bool m_running;
    int m_fixedFpsDelay;
};

} // namespace ds

#endif // DS_ENGINE_H
