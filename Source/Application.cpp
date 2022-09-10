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
    // Vertex buffer
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

    // Vertex array
    Graphics::VertexArray::SetupAttribute vertexAttributes[] =
    {
        // Position
        {
            .buffer = m_vertexBuffer,
            .type = GL_FLOAT,
            .location = 0,
            .count = 3,
            .stride = sizeof(Vertex),
            .offset = offsetof(Vertex, position)
        },
        // Color
        {
            .buffer = m_vertexBuffer,
            .type = GL_FLOAT,
            .location = 1,
            .count = 3,
            .stride = sizeof(Vertex),
            .offset = offsetof(Vertex, color)
        }
    };

    if(!m_vertexArray.Setup(vertexAttributes, std::size(vertexAttributes)))
        return false;

    // Shader
    Graphics::Shader::SetupFromSources shaderParams;

    shaderParams.vertexShader.source =
        "#version 330 core\n"
        "uniform mat4 inTransform;\n"
        "layout(location = 0) in vec3 inPosition;\n"
        "layout(location = 1) in vec3 inColor;\n"
        "out vec4 outColor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = inTransform * vec4(inPosition, 1.0f);\n"
        "    outColor = vec4(inColor, 1.0f);\n"
        "}\n";

    shaderParams.fragmentShader.source =
        "#version 330 core\n"
        "in vec4 inColor;\n"
        "out vec4 outColor;\n"
        "void main()\n"
        "{\n"
        "    outColor = inColor;\n"
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
