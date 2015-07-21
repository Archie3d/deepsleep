#ifndef DS_MESHOBJECT_H
#define DS_MESHOBJECT_H

#include "dsResourceHandle.h"
#include "dsObject.h"

namespace ds {

class MeshObject : public Object
{
public:
    MeshObject(const ResourceHandle &mesh);
    ~MeshObject();

    bool selectForRendering(Renderer *pRenderer);
    void render(Renderer *pRenderer);

private:

    bool initialize();

    ResourceHandle m_meshResourceHandle;
    bool m_initialized;

    int m_nFaces;   ///< Number of faces to draw.
    unsigned m_indexBufferHandle;
    unsigned m_vertexBufferHandle;
    unsigned m_normalBufferHandle;
    unsigned m_vertexArrayHandle;
};

} // namespace ds

#endif // DS_MESHOBJECT_H
