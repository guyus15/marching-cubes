cmake_minimum_required(VERSION 3.22)

# Set the project name
project(marching-cubes)

# Create executable
add_executable(${PROJECT_NAME})

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW build documentation" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW build documentation" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW build documentation" FORCE)

add_subdirectory(thirdparty/glfw)


target_include_directories(${PROJECT_NAME}
    PRIVATE
        include/
        thirdparty/glfw/include
        thirdparty/glad/include
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/application.cpp
        src/world_space.cpp
        thirdparty/glad/src/glad.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES})
