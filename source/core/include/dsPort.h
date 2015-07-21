#ifndef DS_PORT_H
#define DS_PORT_H

#include <memory>
#include "dsVariant.h"

namespace ds {

/**
 * Abstract port.
 */
class Port
{
public:
    Port(const std::string &name);
    std::string name() const { return m_name; }
    virtual Variant value() const = 0;
    virtual ~Port() {}
private:
    std::string m_name;
};

class OutputPort : public Port
{
public:
    OutputPort(const std::string &name = std::string());
    Variant value() const { return m_value; }
    void setValue(const Variant &v) { m_value = v; }

private:
    Variant m_value;
};

typedef std::shared_ptr<OutputPort> OutputPortPtr;

class InputPort : public Port
{
public:
    InputPort(const std::string &name = std::string());
    Variant value() const;
    void connectTo(const OutputPortPtr &outputPortPtr) { m_connectedOutputPortPtr = outputPortPtr; }
private:
    OutputPortPtr m_connectedOutputPortPtr;
};

typedef std::shared_ptr<InputPort> InputPortPtr;

} // namespace ds

#endif // DS_PORT_H
