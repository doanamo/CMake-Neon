#include "Precompiled.hpp"

int main()
{
    if(!glfwInit())
    {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(1024, 576, "Game", nullptr, nullptr);
    if(window == nullptr)
    {
        return 1;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL(glfwGetProcAddress))
    {
        return 1;
    }

    glfwTerminate();
    return 0;
}
