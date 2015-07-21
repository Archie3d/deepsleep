#include "dsVertex.h"

namespace ds {

Vertex::Vertex(const glm::vec3 &pos, const glm::vec3 &norm, const glm::vec2 &uv)
    : m_position(pos),
      m_normal(norm),
      m_uv(uv)
{
}

Vertex::Vertex(const Vertex &v)
    : m_position(v.m_position),
      m_normal(v.m_normal),
      m_uv(v.m_uv)
{
}

Vertex& Vertex::operator =(const Vertex &v)
{
    if (this != &v) {
        m_position = v.m_position;
        m_normal = v.m_normal;
        m_uv = v.m_uv;
    }
    return *this;
}

bool Vertex::operator ==(const Vertex &v) const
{
    return m_position == v.m_position &&
            m_normal == v.m_normal &&
            m_uv == v.m_uv;
}

bool Vertex::operator !=(const Vertex &v) const
{
    return m_position != v.m_position ||
            m_normal != v.m_normal ||
            m_uv != v.m_uv;
}

} // namespace
