#ifndef DS_OBJECTCONTROLLER_H
#define DS_OBJECTCONTROLLER_H

#include "dsIUpdatable.h"

namespace ds {

class Object;

class ObjectController : public IUpdatable
{
public:

    ObjectController();
    virtual ~ObjectController();

    void attachToObject(Object *pObject);
    Object* object() const { return m_pObject; }

private:
    /// Object affected by this controller
    Object *m_pObject;
};

} // namespace ds

#endif // DS_OBJECTCONTROLLER_H
