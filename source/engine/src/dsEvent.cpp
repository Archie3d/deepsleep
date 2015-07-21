#include <SDL.h>
#include "dsEvent.h"

namespace ds {

Event::Event(Type t)
    : m_type(t),
      m_status(Status_Unhandled),
      m_timestamp(SDL_GetTicks())
{
}

Event::~Event()
{
}

} // namespace ds
