#ifndef DS_EVENT_H
#define DS_EVENT_H

#include "dsGlobal.h"

namespace ds {

/**
 * Base class to represent an event.
 */
class Event
{
public:

    enum Type {
        Type_Invalid = 0,
        Type_Update,
        Type_Quit
    };

    enum Status {
        Status_Unhandled,
        Status_Accepted,
        Status_Ignored
    };

    Event(Type t = Type_Invalid);
    virtual ~Event();

    Type type() const { return m_type; }
    Status status() const { return m_status; }
    bool isUnhandled() const { return m_status == Status_Unhandled; }
    void accept() { m_status = Status_Accepted; }
    void ignore() { m_status = Status_Ignored; }
    unsigned int timestamp() const { return m_timestamp; }
    void setTimestamp(unsigned int t) { m_timestamp = t; }

private:
    DS_DISABLE_COPY(Event)

    Type m_type;
    Status m_status;
    unsigned int m_timestamp;

};

} // namespace ds

#endif // DS_EVENT_H
