#include "dsRefCounter.h"
#include "dsResource.h"
#include "dsResourceHandle.h"

namespace ds {

ResourceHandle::ResourceHandle(Resource *pResource)
    : m_pResource(pResource),
      m_pRefCounter(new RefCounter(1))
{
}

ResourceHandle::ResourceHandle(const ResourceHandle &handle)
    : m_pResource(handle.m_pResource),
      m_pRefCounter(handle.m_pRefCounter)
{
    m_pRefCounter->acquire();
}

ResourceHandle& ResourceHandle::operator =(const ResourceHandle &handle)
{
    if (this != &handle) {
        if (m_pRefCounter->release() == 0) {
            delete m_pResource;
            delete m_pRefCounter;
        }

        m_pResource = handle.m_pResource;
        m_pRefCounter = handle.m_pRefCounter;
        m_pRefCounter->acquire();
    }
    return *this;
}

ResourceHandle::~ResourceHandle()
{
    if (m_pRefCounter->release() == 0) {
        dsDebug() << "Resource " <<  m_pResource->name() << " released";
        delete m_pResource;
        delete m_pRefCounter;
    }
}

bool ResourceHandle::operator ==(const ResourceHandle &handle) const
{
    return m_pResource == handle.m_pResource;
}

bool ResourceHandle::operator !=(const ResourceHandle &handle) const
{
    return m_pResource != handle.m_pResource;
}

Resource& ResourceHandle::operator *() const
{
    return *m_pResource;
}

Resource* ResourceHandle::operator ->() const
{
    return m_pResource;
}

} // namespace ds
