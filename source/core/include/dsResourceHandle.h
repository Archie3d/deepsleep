#ifndef DS_RESOURCEHANDLE_H
#define DS_RESOURCEHANDLE_H

namespace ds {

class RefCounter;
class Resource;

/**
 * Handle to a resource.
 * This class implements resource reference counter to
 * handle dynamically the resource lifetime.
 */
class ResourceHandle
{
public:

    ResourceHandle(Resource *pResource = nullptr);
    ResourceHandle(const ResourceHandle &handle);
    ResourceHandle& operator =(const ResourceHandle &handle);
    ~ResourceHandle();

    bool operator ==(const ResourceHandle &handle) const;
    bool operator !=(const ResourceHandle &handle) const;

    Resource& operator *() const;
    Resource* operator ->() const;
    Resource* resource() const { return m_pResource; }
    bool isNull() const { return m_pResource == nullptr; }

    template <typename T>
    T* to() const
    {
        return dynamic_cast<T*>(m_pResource);
    }
private:
    Resource *m_pResource;
    RefCounter *m_pRefCounter;
};

} // namespace ds

#endif // DS_RESOURCEHANDLE_H
