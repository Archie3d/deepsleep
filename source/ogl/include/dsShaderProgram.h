#ifndef DS_SHADERPROGRAM_H
#define DS_SHADERPROGRAM_H

#include <map>
#include "dsGlobal.h"
#include "dsShader.h"
#include "dsShaderUniform.h"

namespace ds {

/**
 * Shader program composed of vertex, fragment and optional
 * geometry shader.
 */
class ShaderProgram
{
public:

    ShaderProgram();
    ~ShaderProgram();

    unsigned handle() const { return m_handle; }
    bool isLinked() const { return m_linked; }

    void attachShader(Shader *pShader);
    bool link();
    void use();

    ShaderUniform& operator[](const std::string &name) { return shaderUniform(name); }
    ShaderUniform& shaderUniform(const std::string &name);

    static ShaderProgram* createFromSource(const std::string &vertexSource,
                                           const std::string &fragmentSource,
                                           const std::string &geometrySource = std::string());

    /**
     * Create a shader program from a single source code string.
     * Shaders are separated by tags @vertex, @geometry, and @fragment.
     * @param source
     * @return
     */
    static ShaderProgram* createFromSource(const std::string &source);

    /**
     * Release shader program (use no program).
     */
    static void release();

private:
    DS_DISABLE_COPY(ShaderProgram)

    unsigned m_handle;
    std::map<Shader::Type, Shader*> m_shaders;
    std::map<std::string, ShaderUniform> m_uniforms;
    bool m_linked;
};

} // namespace ds

#endif // DS_SHADERPROGRAM_H
