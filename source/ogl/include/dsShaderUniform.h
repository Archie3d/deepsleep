#ifndef DS_SHADERUNIFORM_H
#define DS_SHADERUNIFORM_H

#include "dsGlobal.h"
#include "dsMath.h"

namespace ds {

/**
 * GLSL uniform reference.
 */
class ShaderUniform
{
public:
    ShaderUniform();
    ShaderUniform(int location);
    ShaderUniform(const ShaderUniform &uniform);
    ShaderUniform& operator =(const ShaderUniform &uniform);
    int location() const { return m_location; }

    ShaderUniform& operator =(const glm::vec2 &v);
    ShaderUniform& operator =(const glm::vec3 &v);
    ShaderUniform& operator =(const glm::vec4 &v);
    ShaderUniform& operator =(const glm::mat3 &m);
    ShaderUniform& operator =(const glm::mat4 &m);
    ShaderUniform& operator =(float v);
    ShaderUniform& operator =(int v);
    ShaderUniform& operator =(unsigned int v);
    ShaderUniform& operator =(bool v);

private:
    int m_location;
};

} // namespace ds

#endif // DS_SHADERUNIFORM_H
