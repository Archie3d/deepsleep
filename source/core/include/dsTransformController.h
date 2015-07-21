#ifndef DS_TRANSFORMCONTROLLER_H
#define DS_TRANSFORMCONTROLLER_H

#include "dsMath.h"
#include "dsObjectController.h"

namespace ds {

class TransformController : public ObjectController
{
public:
    TransformController();

    void setTransformStep(const glm::mat4x4 &m) { m_transformStep = m; }

    void update(int deltaMs);
private:
    glm::mat4x4 m_transformStep;
};

} // namespace ds

#endif // DS_TRANSFORMCONTROLLER_H
