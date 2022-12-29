#include "resource_manager.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader ResourceManager::load_shader(const std::string& name,
                                    const char* vertex_shader_path,
                                    const char* fragment_shader_path)
{
    s_shaders[name] = load_shader_from_file(vertex_shader_path, fragment_shader_path);
    return s_shaders[name];
}

Shader ResourceManager::get_shader(const std::string& name)
{
    return s_shaders[name];
}

Shader ResourceManager::load_shader_from_file(const char* vertex_shader_path,
                                              const char* fragment_shader_path)
{
    std::string vertex_code, fragment_code;
    std::ifstream vertex_shader_file, fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);

        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "Error: Shader file could not be read successfully.\n" << e.what() << std::endl;
    }

    Shader shader;
    shader.compile(vertex_code, fragment_code);

    return shader;
}

std::unordered_map<std::string, Shader> ResourceManager::s_shaders;
