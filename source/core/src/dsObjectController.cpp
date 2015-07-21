#include "dsObject.h"
#include "dsObjectController.h"

namespace ds {

ObjectController::ObjectController()
    : m_pObject(nullptr)
{
}

ObjectController::~ObjectController()
{
    if (m_pObject != nullptr) {
        m_pObject->detachControllerPrivate(this);
    }
}

void ObjectController::attachToObject(Object *pObject)
{
    DS_ASSERT(pObject != nullptr);
    pObject->attachControllerPrivate(this);
    m_pObject = pObject;
}

} // namespace ds
