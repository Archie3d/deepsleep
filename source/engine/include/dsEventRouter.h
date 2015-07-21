#ifndef DS_EVENTROUTER_H
#define DS_EVENTROUTER_H

#include <vector>
#include <map>
#include "dsTypeInfo.h"
#include "dsEvent.h"

namespace ds {

/**
 * Gateway to an event handler.
 */
class IEventGateway
{
public:
    virtual ~IEventGateway() {}
    void doHandleEvent(Event *pEvent) { handleEvent(pEvent); }
protected:
    virtual void handleEvent(Event *pEvent) = 0;
};

/**
 * Handler that links an event to class memeber function.
 */
template <class T, class EventT>
class EventGateway : public IEventGateway
{
public:
    EventGateway(T *pInstance, void (T::*pMember)(EventT*))
        : m_pInstance(pInstance),
          m_function(pMember)
    { }

    void handleEvent(Event *pEvent)
    {
        (m_pInstance->*m_function)(static_cast<EventT*>(pEvent));
    }

private:
    T *m_pInstance;
    void (T::*m_function)(EventT*);
};

/**
 * Base class for event handlers.
 */
class EventHandler
{
public:

    EventHandler()
        : m_handlers()
    {
    }

    virtual ~EventHandler()
    {
        Handlers::iterator it = m_handlers.begin();
        while (it != m_handlers.end()) {
            delete it->second;
            ++it;
        }
        m_handlers.clear();
    }

    template <class T, class EventT>
    void registerEventFunc(T* pObj, void (T::*pMember)(EventT*))
    {
        m_handlers[TypeInfo(typeid(EventT))] = new EventGateway<T, EventT>(pObj, pMember);
    }

    void handleEvent(Event *pEvent)
    {
        Handlers::iterator it = m_handlers.find(TypeInfo(typeid(*pEvent)));
        if (it != m_handlers.end()) {
            it->second->doHandleEvent(pEvent);
        }
    }

private:
    typedef std::map<TypeInfo, IEventGateway*> Handlers;
    Handlers m_handlers;
};

class EventRouter
{
public:
    EventRouter();
    ~EventRouter();

    /**
     * Add event handler to routing.
     * @param pHandler
     */
    void addEventHandler(EventHandler *pHandler);

    /**
     * Remove event handler from routing.
     * @param pHandler
     */
    void removeEventHandler(EventHandler *pHandler);

    /**
     * Send event directly to all handlers.
     * @param pEvent
     */
    void sendEvent(Event *pEvent);

    /**
     * Post event to the event queue.
     * @param pEvent
     */
    void postEvent(Event *pEvent);

    /**
     * Dispatch all pending events.
     */
    void dispatchEvents();

    /**
     * Remove all pending events.
     */
    void purge();

    /**
     * Fetch all available events and dispatch them to handlers.
     */
    void collectEndDispatchEvents();

protected:

    static Event* pollEvent();

private:
    std::vector<EventHandler*> m_handlers;
    std::vector<Event*> m_events;   ///< Event queue.
};

} // namespace ds

#endif // DS_EVENTROUTER_H
