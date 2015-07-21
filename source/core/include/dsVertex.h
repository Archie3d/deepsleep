#ifndef DS_VERTEX_H
#define DS_VERTEX_H

#include "dsMath.h"
#include "dsISerializable.h"

namespace ds {

/**
 * Vertex carrying related attributes:
 *  - position in 3D space,
 *  - normal vector,
 *  - UV texture coordinates
 */
class Vertex : public ISerializable
{
public:

    Vertex(const glm::vec3 &pos = glm::vec3(0.0f, 0.0f, 0.0f),
           const glm::vec3 &norm = glm::vec3(0.0f, 0.0f, 1.0f),
           const glm::vec2 &uv = glm::vec2(0.0f, 0.0f));
    Vertex(const Vertex &v);
    Vertex& operator =(const Vertex &v);

    bool operator ==(const Vertex &v) const;
    bool operator !=(const Vertex &v) const;

    void setPosition(const glm::vec3 &p) { m_position = p; }
    void setNormal(const glm::vec3 &p) { m_normal = p; }
    void setUV(const glm::vec2 &p) { m_uv = p; }

    const glm::vec3& position() const { return m_position; }
    const glm::vec3& normal() const { return m_normal; }
    const glm::vec2& uv() const { return m_uv; }

private:
    glm::vec3 m_position;   ///< Vertex position.
    glm::vec3 m_normal;     ///< Vertex normal.
    glm::vec2 m_uv;         ///< Texture coordinates.
};

} // namespace ds

#endif // DS_VERTEX_H
