#include "Precompiled.hpp"
#include "Application.hpp"

struct Vertex
{
    Eigen::Vector3f position;
    Eigen::Vector3f color;
};

Application::Application(GLFWwindow* window)
    : m_window(window)
{
}

bool Application::Setup()
{
    return true;
}

void Application::Update(float deltaTime)
{
    UNUSED(deltaTime);
}

void Application::Render(float alphaTime)
{
    UNUSED(alphaTime);
}

void Application::OnKeyboardKey(KeyboardKeyEvent event)
{
    if(event.key == GLFW_KEY_ESCAPE && event.action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}
