#include <algorithm>
#include <SDL.h>
#include "dsQuitEvent.h"
#include "dsEventRouter.h"

namespace ds {

EventRouter::EventRouter()
    : m_handlers(),
      m_events()
{
}

EventRouter::~EventRouter()
{
}

void EventRouter::addEventHandler(EventHandler *pHandler)
{
    m_handlers.push_back(pHandler);
}

void EventRouter::removeEventHandler(EventHandler *pHandler)
{
    std::vector<EventHandler*>::iterator it = std::find(m_handlers.begin(), m_handlers.end(), pHandler);
    if (it != m_handlers.end()) {
        m_handlers.erase(it);
    }
}

void EventRouter::sendEvent(Event *pEvent)
{
    DS_ASSERT(pEvent != nullptr);
    std::vector<EventHandler*>::iterator it = m_handlers.begin();
    while (it != m_handlers.end() && pEvent->isUnhandled()) {
        (*it)->handleEvent(pEvent);
        ++it;
    }
}

void EventRouter::postEvent(Event *pEvent)
{
    DS_ASSERT(pEvent != nullptr);
    m_events.push_back(pEvent);
}

void EventRouter::dispatchEvents()
{
    std::vector<Event*> events = m_events;
    m_events.clear();

    std::vector<Event*>::iterator it = events.begin();
    while (it != events.end()) {
        Event *pEvent = *it;
        sendEvent(pEvent);
        delete pEvent;
        ++it;
    }
}

void EventRouter::purge()
{
    std::vector<Event*>::iterator it = m_events.begin();
    while (it != m_events.end()) {
        Event *pEvent = *it;
        delete pEvent;
        ++it;
    }
    m_events.clear();
}

void EventRouter::collectEndDispatchEvents()
{
    Event *pEvent = nullptr;
    while ((pEvent = EventRouter::pollEvent()) != nullptr) {
        postEvent(pEvent);
    }
    dispatchEvents();
}

Event* EventRouter::pollEvent()
{
    Event *pEvent = nullptr;
    SDL_Event sdlEvent;
    if (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
        case SDL_QUIT:
            pEvent = new QuitEvent();
            break;
        default:
            break;
        }
    }
    return pEvent;
}

} // namespace ds
