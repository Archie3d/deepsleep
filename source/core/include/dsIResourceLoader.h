#ifndef DS_IRESOURCELOADER_H
#define DS_IRESOURCELOADER_H

#include <string>

namespace ds {

class Resource;

/**
 * Interface to a resource loader.
 * Resource loader handles loading resources by their names.
 */
class IResourceLoader
{
public:

    virtual Resource* load(const std::string &name) = 0;
    virtual ~IResourceLoader() {}
};

} // namespace ds

#endif // DS_IRESOURCELOADER_H
