#include <glew.h>
#include <SDL.h>
#include "dsTexture.h"

namespace ds {

Texture::Texture(const std::string &name)
    : Resource(Type_Texture, name),
      m_handle(0)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_handle);
}

bool Texture::isValid() const
{
    return m_handle != 0;
}

Texture* Texture::loadFromFile(const std::string &name, const std::string &path)
{
    SDL_Surface *pSdlSurface = SDL_LoadBMP(path.c_str());
    if (pSdlSurface == nullptr) {
        return nullptr;
    }

    Texture *pTexture = new Texture(name);
    pTexture->initializeFromSurface(pSdlSurface);
    SDL_FreeSurface(pSdlSurface);
    return pTexture;
}

void Texture::initializeFromSurface(SDL_Surface *pSurface)
{
    DS_ASSERT(m_handle == 0);
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 pSurface->w, pSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, pSurface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

} // namespace ds
