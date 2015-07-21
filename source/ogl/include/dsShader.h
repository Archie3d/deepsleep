#ifndef DS_SHADER_H
#define DS_SHADER_H

#include <string>
#include "dsGlobal.h"

namespace ds {

/**
 * GLSL shader.
 */
class Shader
{
public:

    /// Shader type.
    enum Type {
        Type_Fragment,
        Type_Vertex,
        Type_Geometry
    };

    Shader(Type type);
    virtual ~Shader();

    Type type() const { return m_type; }

    void setSource(const std::string &source);
    bool compile();
    bool isCompiled() const { return m_compiled; }
    unsigned handle() const { return m_handle; }

private:
    DS_DISABLE_COPY(Shader)

    Type m_type;
    unsigned m_handle;
    bool m_compiled;
};

class FragmentShader : public Shader
{
public:
    FragmentShader();
};

class VertexShader : public Shader
{
public:
    VertexShader();
};

class GeometryShader : public Shader
{
public:
    GeometryShader();
};

} // namespace ds

#endif // DS_SHADER_H
