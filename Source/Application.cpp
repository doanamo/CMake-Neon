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
    static const char* vertexShader =
        "#version 330\n"
        "uniform mat4 inTransform;\n"
        "attribute vec2 inPosition;\n"
        "attribute vec3 inColor;\n"
        "varying vec3 outColor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = inTransform * vec4(inPosition, 0.0, 1.0);\n"
        "    outColor = inColor;\n"
        "}\n";

    static const char* fragmentShader =
        "#version 330\n"
        "varying vec3 inColor;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(inColor, 1.0);\n"
        "}\n";

    Graphics::Shader::LoadFromSources shaderParams;
    shaderParams.SetSource(Graphics::ShaderType::VertexShader, vertexShader);
    shaderParams.SetSource(Graphics::ShaderType::FragmentShader, fragmentShader);
    if(!m_shader.Setup(shaderParams))
        return false;

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
