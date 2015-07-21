#ifndef DS_RESOURCECACHE_H
#define DS_RESOURCECACHE_H

#include <list>
#include <map>
#include "dsGlobal.h"
#include "dsResourceHandle.h"

namespace ds {

class IResourceLoader;

class ResourceCache
{
public:

    ResourceCache(IResourceLoader *pLoader);
    ~ResourceCache();

    /**
     * Get a resource by its name.
     * @param name
     * @return
     */
    ResourceHandle get(const std::string &name);

    size_t memoryUsage() const { return m_allocated; }
    void decrementAllocation(size_t size);
protected:

    /**
     * Move resource on the top of usage list.
     * @param handle
     */
    void promoteResource(const ResourceHandle &handle);

    /**
     * Releases one less frequently used resource
     * (at the end of the list).
     */
    void releaseLessFrequentResource();

private:
    DS_DISABLE_COPY(ResourceCache)

    typedef std::list<ResourceHandle> ResourceHandlesList;
    typedef std::map<std::string, ResourceHandle> ResourceHandlesMap;

    IResourceLoader *m_pResourceLoader;
    ResourceHandlesList m_resourceHandlesList;
    ResourceHandlesMap m_resourceHandlesMap;

    size_t m_allocated; ///< Allocated bytes.
};

} // namespace ds

#endif // DS_RESOURCECACHE_H
