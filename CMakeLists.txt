cmake_minimum_required(VERSION 3.22)

# Set the project name
project(marching-cubes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY out)

# Create executable
add_executable(${PROJECT_NAME})

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW build documentation" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW build documentation" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW build documentation" FORCE)

add_subdirectory(thirdparty/glfw)

# glm
add_subdirectory(thirdparty/glm)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include/
        thirdparty/glfw/include
        thirdparty/glad/include
        thirdparty/glm/
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/application.cpp
        src/world_space.cpp
        src/shader.cpp
        src/resource_manager.cpp
        thirdparty/glad/src/glad.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES})

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
    COMMENT "Copying resources into build directory")

add_dependencies(${PROJECT_NAME} copy_resources)
