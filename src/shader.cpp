#include "shader.h"

#include "glm/gtc/type_ptr.hpp"

#include <iostream>

Shader::Shader()
    : m_compiled{ false }
{
}

void Shader::compile(const std::string& vertex_shader_code,
                     const std::string& fragment_shader_code)
{   
    const char* vertex_code_c = vertex_shader_code.c_str();
    const char* fragment_code_c = fragment_shader_code.c_str();

    int success = 0;
    char info_log[512];
    GLint vertex_shader, fragment_shader;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_code_c, nullptr);
    glShaderSource(fragment_shader, 1, &fragment_code_c, nullptr);

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        std::cerr << "Error: Vertex shader compilation failed.\n" << info_log << std::endl;
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        std::cerr << "Error: Fragment shader compilation failed.\n" << info_log << std::endl;
    }

    m_id = glCreateProgram();

    glAttachShader(m_id, vertex_shader);
    glAttachShader(m_id, fragment_shader);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 512, nullptr, info_log);
        std::cerr << "Error: Failed to link shader program.\n" << info_log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    m_compiled = true;
}

void Shader::use() const
{
    if (!m_compiled)
    {
        std::cerr << "Shader not used: the shader has not been compiled." << std::endl;
        return;
    }

    glUseProgram(m_id);
}

void Shader::set_bool(const std::string& name, bool value) const
{   
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string& name, int value) const
{   
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_vec3(const std::string& name, float value1, float value2, float value3) const
{
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), value1, value2, value3);
}

void Shader::set_vec3(const std::string& name, glm::vec3& value) const
{
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
}

void Shader::set_mat4(const std::string& name, glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
