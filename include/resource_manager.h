#ifndef RESOURCE_MANAGER_
#define RESOURCE_MANAGER_

#include "shader.h"

#include <string>
#include <unordered_map>

class ResourceManager
{
public:
    static Shader load_shader(const std::string& name,
                              const char* vertex_shader_path,
                              const char* fragment_shader_path);
    static Shader get_shader(const std::string& name);

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    static Shader load_shader_from_file(const char* vertex_shader_path,
                                          const char* fragment_shader_path);

    static std::unordered_map<std::string, Shader> s_shaders;
};

#endif // RESOURCE_MANAGER_
