#include <glew.h>
#include "dsShaderUniform.h"

namespace ds {

ShaderUniform::ShaderUniform()
    : m_location(-1)
{
}

ShaderUniform::ShaderUniform(int location)
    : m_location(location)
{
}

ShaderUniform::ShaderUniform(const ShaderUniform &uniform)
    : m_location(uniform.m_location)
{
}

ShaderUniform& ShaderUniform::operator =(const ShaderUniform &uniform)
{
    if (this != &uniform) {
        m_location = uniform.m_location;
    }
    return *this;
}

ShaderUniform& ShaderUniform::operator =(const glm::vec2 &v)
{
    glUniform2f(m_location, v.x, v.y);
    return *this;
}

ShaderUniform& ShaderUniform::operator =(const glm::vec3 &v)
{
    glUniform3f(m_location, v.x, v.y, v.z);
    return *this;
}

ShaderUniform& ShaderUniform::operator =(const glm::vec4 &v)
{
    glUniform4f(m_location, v.x, v.y, v.z, v.w);
    return *this;
}

ShaderUniform& ShaderUniform::operator =(const glm::mat3 &m)
{
    glUniformMatrix3fv(m_location, 1, GL_FALSE /* do not transpose */, glm::value_ptr(m));
    return *this;
}

ShaderUniform& ShaderUniform::operator =(const glm::mat4 &m)
{
    glUniformMatrix4fv(m_location, 1, GL_FALSE /* do not transpose */, glm::value_ptr(m));
    return *this;
}

ShaderUniform& ShaderUniform::operator =(float v)
{
    glUniform1f(m_location, v);
    return *this;
}

ShaderUniform& ShaderUniform::operator =(int v)
{
    glUniform1i(m_location, v);
    return *this;
}

ShaderUniform& ShaderUniform::operator =(unsigned int v)
{
    glUniform1ui(m_location, v);
    return *this;
}

ShaderUniform& ShaderUniform::operator =(bool v)
{
    glUniform1i(m_location, v ? 1 : 0);
    return *this;
}

} // namespace ds
