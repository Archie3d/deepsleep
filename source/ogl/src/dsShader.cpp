#include <glew.h>
#include "dsShader.h"

namespace ds {

GLenum shaderByType(Shader::Type type)
{
    switch (type) {
    case Shader::Type_Fragment:
        return GL_FRAGMENT_SHADER;
    case Shader::Type_Vertex:
        return GL_VERTEX_SHADER;
    case Shader::Type_Geometry:
        return GL_GEOMETRY_SHADER;
    default:
        DS_ASSERT(!"Invalid shader type");
    }
    return GL_FRAGMENT_SHADER;
}

//
// Shader implementation
//

Shader::Shader(Type type)
    : m_type(type),
      m_handle(0),
      m_compiled(false)
{
    m_handle = glCreateShader(shaderByType(type));
    DS_ASSERT(m_handle != 0);
}

Shader::~Shader()
{
    glDeleteShader(m_handle);
}

void Shader::setSource(const std::string &source)
{
    if (m_compiled) {
        dsError() << "Shader has been already compiler, cannot set source code.";
        return;
    }

    const char *src = source.c_str();

    glShaderSource(m_handle, 1, &src, 0);
}

bool Shader::compile()
{
    if (m_compiled) {
        dsWarning() << "Shader has been already compiled";
        return false;
    }

    glCompileShader(m_handle);

    // Fetch compilation results
    int result;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result) {
        // Compilation failed.
        int length = 0;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);
        dsError() << "Shader compilation failed";
        if (length > 0) {
            char *cLog = new char[length];
            glGetShaderInfoLog(m_handle, length, nullptr, cLog);
            dsError() << cLog;
            delete[] cLog;
        }
        return false;
    }

    // Compilation went Ok.
    m_compiled = true;
    return true;
}

//
// FragmentShader implementation
//

FragmentShader::FragmentShader()
    : Shader(Type_Fragment)
{
}

//
// VertexShader
//

VertexShader::VertexShader()
    : Shader(Type_Vertex)
{
}

//
// GeometryShader
//

GeometryShader::GeometryShader()
    : Shader(Type_Geometry)
{
}

} // namespace ds
