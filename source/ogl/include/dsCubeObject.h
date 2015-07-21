#ifndef DS_CUBEOBJECT_H
#define DS_CUBEOBJECT_H

#include "dsObject.h"

namespace ds {

class CubeObject : public Object
{
public:

    CubeObject();
    ~CubeObject();

    bool selectForRendering(Renderer *pRenderer);
    void render(Renderer *pRenderer);

private:

    bool initialize();

    bool m_initialized;
    unsigned m_vertexArrayHandle;
    unsigned m_vertexBufferHandle;
    unsigned m_normalBufferHandle;
    unsigned m_indexBufferHandle;
};

} // namespace ds

#endif // DS_CUBEOBJECT_H
