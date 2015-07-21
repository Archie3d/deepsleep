#include "dsPort.h"

namespace ds {

Port::Port(const std::string &name)
    : m_name(name)
{
}

OutputPort::OutputPort(const std::string &name)
    : Port(name),
      m_value()
{
}

InputPort::InputPort(const std::string &name)
    : Port(name),
      m_connectedOutputPortPtr(nullptr)
{
}

Variant InputPort::value() const
{
    if (m_connectedOutputPortPtr == nullptr) {
        return Variant();
    }
    return m_connectedOutputPortPtr->value();
}

} // namespace ds
