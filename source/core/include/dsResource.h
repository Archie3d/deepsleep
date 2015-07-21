#ifndef DS_RESOURCE_H
#define DS_RESOURCE_H

#include <string>
#include "dsGlobal.h"

namespace ds {

class ResourceCache;

/**
 * Abstract resource.
 */
class Resource
{
    friend class ResourceCache;
public:

    enum Type {
        Type_Unknown,
        Type_Mesh,
        Type_Texture
    };

    Resource(Type type, const std::string &name);
    virtual ~Resource();

    Type type() const { return m_type; }
    std::string name() const { return m_name; }

    ResourceCache* cache() const { return m_pResourceCache; }

    virtual bool isValid() const { return false; }

    /**
     * Tells how much memory this resource occupies.
     * @return
     */
    virtual size_t memoryUsage() const { return 0; }

private:

    void setResourceCache(ResourceCache *pCache) { m_pResourceCache = pCache; }

private:
    DS_DISABLE_COPY(Resource)

    Type m_type;        ///< Resource type.
    std::string m_name; ///< Resource name.
    ResourceCache *m_pResourceCache;
};

} // namespace

#endif // DS_RESOURCE_H
