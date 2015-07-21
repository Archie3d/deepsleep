#include "dsResourceCache.h"
#include "dsResource.h"

namespace ds {

Resource::Resource(Type type, const std::string &name)
    : m_type(type),
      m_name(name),
      m_pResourceCache(nullptr)
{
}

Resource::~Resource()
{
    if (m_pResourceCache != nullptr) {
        m_pResourceCache->decrementAllocation(memoryUsage());
    }
}

} // namespace ds
