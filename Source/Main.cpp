#include "Precompiled.hpp"

int main()
{
    Logger::Setup();

    Eigen::Vector3f position{ 1.0f, 2.0f, 3.0f };
    fmt::print("Hello {}", "world!");

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
