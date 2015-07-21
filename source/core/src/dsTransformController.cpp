#include "dsObject.h"
#include "dsTransformController.h"

namespace ds {

TransformController::TransformController()
    : ObjectController(),
      m_transformStep(1.0f)
{
}

void TransformController::update(int deltaMs)
{
    Object *pObject = object();
    if (pObject != nullptr) {
        pObject->setTransformation(pObject->transformation() * m_transformStep);
    }
}

} // namespace ds
