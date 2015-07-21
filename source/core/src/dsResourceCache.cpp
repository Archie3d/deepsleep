#include "dsIResourceLoader.h"
#include "dsResource.h"
#include "dsResourceCache.h"

namespace ds {

ResourceCache::ResourceCache(IResourceLoader *pLoader)
    : m_pResourceLoader(pLoader)
{
    DS_ASSERT(pLoader != nullptr);
}

ResourceCache::~ResourceCache()
{
    delete m_pResourceLoader;
}

ResourceHandle ResourceCache::get(const std::string &name)
{
    ResourceHandlesMap::iterator it = m_resourceHandlesMap.find(name);
    if (it != m_resourceHandlesMap.end()) {
        ResourceHandle handle = it->second;
        promoteResource(handle);
        return handle;
    }

    // Load resource
    Resource *pResource = m_pResourceLoader->load(name);
    ResourceHandle handle(pResource);
    if (handle.isNull()) {
        dsError() << "Unable to load resource " << name;
        return handle;
    }

    m_resourceHandlesList.push_front(handle);
    m_resourceHandlesMap[name] = handle;
    m_allocated += pResource->memoryUsage();
    pResource->setResourceCache(this);

    return handle;
}

void ResourceCache::decrementAllocation(size_t size)
{
    DS_ASSERT(m_allocated >= size);
    m_allocated -= size;
}

void ResourceCache::promoteResource(const ResourceHandle &handle)
{
    m_resourceHandlesList.remove(handle);
    m_resourceHandlesList.push_front(handle);
}

void ResourceCache::releaseLessFrequentResource()
{
    if (m_resourceHandlesList.size() == 0) {
        // Nothing to release.
        return;
    }

    ResourceHandlesList::iterator it = m_resourceHandlesList.end();
    --it;
    ResourceHandle handle = *it;
    m_resourceHandlesList.erase(it);
    ResourceHandlesMap::iterator mit = m_resourceHandlesMap.begin();
    while (mit != m_resourceHandlesMap.end()) {
        if (mit->second == handle) {
            m_resourceHandlesMap.erase(mit);
            break;
        }
        ++mit;
    }
}

} // namespace ds
