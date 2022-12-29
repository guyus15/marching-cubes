#ifndef SHADER_
#define SHADER_

#include "glad/glad.h"
#include "glm/mat4x4.hpp"

#include <string>

class Shader
{
public:
    Shader();

    void compile(const std::string& vertex_shader_code,
                 const std::string& fragment_shade_code);
    void use() const;

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec3(const std::string& name, float value1, float value2, float value3) const;
    void set_vec3(const std::string& name, glm::vec3& vec3) const;
    void set_mat4(const std::string& name, glm::mat4& mat4) const;

private:
    GLint m_id;
    bool m_compiled;
};

#endif // SHADER_
