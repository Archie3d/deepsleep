#ifndef DS_TEXTURE_H
#define DS_TEXTURE_H

#include "dsResource.h"

struct SDL_Surface;

namespace ds {

class Texture : public Resource
{
public:
    Texture(const std::string &name);
    ~Texture();

    unsigned handle() const { return m_handle; }

    bool isValid() const;

    static Texture* loadFromFile(const std::string &name, const std::string &path);

protected:

    void initializeFromSurface(SDL_Surface *pSurface);

private:
    unsigned m_handle;  ///< OpenGL texture handle.
};

} // namespace ds

#endif // DS_TEXTURE_H
