#include <algorithm>
#include <glew.h>
#include "dsShaderProgram.h"

namespace ds {

ShaderProgram::ShaderProgram()
    : m_handle(0),
      m_shaders(),
      m_uniforms(),
      m_linked(false)
{
    m_handle = glCreateProgram();
    DS_ASSERT(m_handle != 0);
}

ShaderProgram::~ShaderProgram()
{
    // Delete shaders
    for (std::map<Shader::Type, Shader*>::iterator it = m_shaders.begin();
         it != m_shaders.end();
         ++it) {
        Shader *pShader = it->second;
        delete pShader;
    }
    glDeleteProgram(m_handle);
}

void ShaderProgram::attachShader(Shader *pShader)
{
    DS_ASSERT(pShader != nullptr);
    if (m_shaders.find(pShader->type()) != m_shaders.end()) {
        // Detach previous shader
        Shader *pPrevShader = m_shaders[pShader->type()];
        delete pPrevShader;
        m_shaders.erase(pShader->type());
    }

    m_shaders[pShader->type()] = pShader;
    if (pShader->compile()) {
        glAttachShader(m_handle, pShader->handle());
    }

    m_linked = false;
}

bool ShaderProgram::link()
{
    if (m_linked) {
        dsWarning() << "Shader program has been already linked";
        return true;
    }

    glLinkProgram(m_handle);

    int result = 0;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &result);
    if (GL_FALSE == result) {
        // Link failed.
        dsError() << "Shader program link failed";

        int length = 0;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            char *cLog = new char[length];
            glGetProgramInfoLog(m_handle, length, nullptr, cLog);
            dsError() << cLog;
            delete[] cLog;
        }

        return false;
    }

    // Linked Ok.
    m_linked = true;
    return true;
}

void ShaderProgram::use()
{
    if (!m_linked) {
        if (!link()) {
            return;
        }
    }
    glUseProgram(m_handle);
}

ShaderUniform& ShaderProgram::shaderUniform(const std::string &name)
{
    std::map<std::string, ShaderUniform>::iterator pos = m_uniforms.find(name);
    if (pos == m_uniforms.end()) {
        // Find uniform variable and cache it
        int location = glGetUniformLocation(m_handle, name.c_str());
        if (location < 0) {
            dsError() << "Unable to locate uniform " << name;
        }
        m_uniforms[name] = ShaderUniform(location);
    }
    return m_uniforms[name];
}

ShaderProgram* ShaderProgram::createFromSource(const std::string &vertexSource,
                                               const std::string &fragmentSource,
                                               const std::string &geometrySource)
{
    ShaderProgram *pShaderProgram = new ShaderProgram();
    Shader *pShader = new VertexShader();
    pShader->setSource(vertexSource);
    pShaderProgram->attachShader(pShader);

    pShader = new FragmentShader();
    pShader->setSource(fragmentSource);
    pShaderProgram->attachShader(pShader);

    if (!geometrySource.empty()) {
        pShader = new GeometryShader();
        pShader->setSource(geometrySource);
        pShaderProgram->attachShader(pShader);
    }

    if (!pShaderProgram->link()) {
        delete pShaderProgram;
        return nullptr;
    }

    return pShaderProgram;
}

ShaderProgram* ShaderProgram::createFromSource(const std::string &source)
{
    std::string vertexSource;
    std::string fragmentSource;
    std::string geometrySource;

    std::string *pCodeSource = nullptr;

    std::vector<std::string> lines = split(source, "\n");
    std::vector<std::string>::const_iterator it = lines.begin();
    while (it != lines.end()) {
        std::string line = trim(*it);
        ++it;

        if (line.length() > 0 && line.at(0) == '@') {
            if (line == "@vertex") {
                pCodeSource = &vertexSource;
            } else if (line == "@geometry") {
                pCodeSource = &geometrySource;
            } else if (line == "@fragment") {
                pCodeSource = &fragmentSource;
            }

            continue;
        }

        if (pCodeSource != nullptr) {
            pCodeSource->append(line);
            pCodeSource->append("\n");
        }
    }

    return createFromSource(vertexSource, fragmentSource, geometrySource);
}

void ShaderProgram::release()
{
    glUseProgram(0);
}

} // namespace ds
