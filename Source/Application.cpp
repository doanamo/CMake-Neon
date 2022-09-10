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
    Vertex vertices[] =
    {
        { { -0.6f, -0.4f, 0.0f }, { 1.0f, 0.0f, 0.0f  } },
        { {  0.6f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f  } },
        { {  0.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f  } }
    };

    Graphics::Buffer::SetupFromParams vertexBufferParams
    {
        .type = GL_ARRAY_BUFFER,
        .usage = GL_STATIC_DRAW,
        .elementSize = sizeof(Vertex),
        .elementCount = std::size(vertices),
        .data = &vertices[0]
    };

    if(!m_vertexBuffer.Setup(vertexBufferParams))
        return false;

    Graphics::Shader::SetupFromSources shaderParams;

    shaderParams.vertexShader.source =
        "#version 330\n"
        "uniform mat4 inTransform;\n"
        "attribute vec3 inPosition;\n"
        "attribute vec3 inColor;\n"
        "varying vec3 outColor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = inTransform * vec4(inPosition, 1.0f);\n"
        "    outColor = inColor;\n"
        "}\n";

    shaderParams.fragmentShader.source =
        "#version 330\n"
        "varying vec3 inColor;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(inColor, 1.0f);\n"
        "}\n";

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
