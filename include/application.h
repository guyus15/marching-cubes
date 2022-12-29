#ifndef APPLICATION_
#define APPLICATION_

#include "world_space.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Application
{
public:
    Application();
    ~Application();

    void Initialise();
    void Run();    

private:
    GLFWwindow* m_window;
    WorldSpace m_world_space;
};

#endif // APPLICATION_
