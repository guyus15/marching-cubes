#include "application.h"
#include "resource_manager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

const std::string APP_NAME = "Marching Cubes";
const uint32_t SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

Application::Application()
    : m_window{ nullptr }
{
    Initialise();
}

Application::~Application()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::Initialise()
{
    if (!glfwInit())
    {
        std::cerr << "Error: Failed to initialise GLFW." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME.c_str(), nullptr, nullptr);

    if (!m_window)
    {
        std::cerr << "Error: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Error: Failed to initialise GLAD." << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Load shaders
    ResourceManager::load_shader("world_shader", "resources/shaders/world_shader.vs", "resources/shaders/world_shader.fs");

    // Generate the world space.
    m_world_space.Generate();
}

void Application::Run()
{
    std::vector<Cube> cubes = m_world_space.GetCubes();
    
    float vertices[WIDTH * HEIGHT * DEPTH * 8 * 3];

    int current_vertex = 0;
    int current_cube = 0;

    for (const auto& cube : cubes)
    {
        vertices[current_vertex++] = cube.vertices[0].x;
        vertices[current_vertex++] = cube.vertices[0].y;
        vertices[current_vertex++] = cube.vertices[0].z;

        vertices[current_vertex++] = cube.vertices[1].x;
        vertices[current_vertex++] = cube.vertices[1].y;
        vertices[current_vertex++] = cube.vertices[1].z;

        vertices[current_vertex++] = cube.vertices[2].x;
        vertices[current_vertex++] = cube.vertices[2].y;
        vertices[current_vertex++] = cube.vertices[2].z;

        vertices[current_vertex++] = cube.vertices[3].x;
        vertices[current_vertex++] = cube.vertices[3].y;
        vertices[current_vertex++] = cube.vertices[3].z;

        vertices[current_vertex++] = cube.vertices[4].x;
        vertices[current_vertex++] = cube.vertices[4].y;
        vertices[current_vertex++] = cube.vertices[4].z;

        vertices[current_vertex++] = cube.vertices[5].x;
        vertices[current_vertex++] = cube.vertices[5].y;
        vertices[current_vertex++] = cube.vertices[5].z;

        vertices[current_vertex++] = cube.vertices[6].x;
        vertices[current_vertex++] = cube.vertices[6].y;
        vertices[current_vertex++] = cube.vertices[6].z;

        vertices[current_vertex++] = cube.vertices[7].x;
        vertices[current_vertex++] = cube.vertices[7].y;
        vertices[current_vertex++] = cube.vertices[7].z;
    }

    // Set up polygon vertices with some method. And load them into a separate vertex buffer with a
    // different vertex array object.
    std::vector<float> polygon_vertices = m_world_space.CreateTriangles();

    // TODO: now vertices are loaded into buffer. We can send them off to the shader
    // to be drawn!
    
    unsigned int VAOs[2], VBOs[2];

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // Grid buffer
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    // Polygon buffer
    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, polygon_vertices.size() * sizeof(float), &polygon_vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    // Unbind
    glBindVertexArray(0);

    Shader world_shader = ResourceManager::get_shader("world_shader");

    glm::mat4 model{ 1.0f };
    glm::mat4 view{ 1.0f };
    glm::mat4 projection{ 1.0f }; 

    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw mesh
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        
        model = glm::mat4{ 1.0f };
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * 10.0f), glm::vec3{ 0.0f, 1.0f, 0.0f });
        model = glm::scale(model, glm::vec3{ 0.1f, 0.1f, 0.1f });

        view = glm::mat4{ 1.0f };
        view = glm::translate(view, glm::vec3{ 0.0f, 0.0f, -0.5f });

        float aspect_ratio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

        projection = glm::perspective(45.0f, aspect_ratio, 0.1f, 100.0f);

        world_shader.use();
        world_shader.set_mat4("model", model);
        world_shader.set_mat4("view", view);
        world_shader.set_mat4("projection", projection);

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_POINTS, 0, WIDTH * HEIGHT * DEPTH * 8);

        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, polygon_vertices.size() / 3);

        // Unbind
        glBindVertexArray(0);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
