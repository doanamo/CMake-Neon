#include "Precompiled.hpp"
#include "Application.hpp"

Application::Application(GLFWwindow* window)
    : m_window(window)
{
}

bool Application::Setup()
{
    // Vertex buffer
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    Vertex vertices[] =
    {
        { {  0.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f  } },
        { {  0.6f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f  } },
        { { -0.6f, -0.4f, 0.0f }, { 1.0f, 0.0f, 0.0f  } }
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

    // Index buffer
    uint16_t indices[] =
    {
        2, 1, 0
    };

    Graphics::Buffer::SetupFromParams indexBufferParams
    {
        .type = GL_ELEMENT_ARRAY_BUFFER,
        .usage = GL_STATIC_DRAW,
        .elementSize = sizeof(short),
        .elementCount = std::size(indices),
        .data = &indices[0]
    };

    if(!m_indexBuffer.Setup(indexBufferParams))
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
    Graphics::Shader::SetupFromSources shaderParams =
    {
        .vertexShader =
        {
            .source =
                "#version 330 core\n"
                "uniform mat4 vertTransform;\n"
                "layout(location = 0) in vec3 vertPosition;\n"
                "layout(location = 1) in vec3 vertColor;\n"
                "out vec4 fragColor;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = vertTransform * vec4(vertPosition, 1.0f);\n"
                "    fragColor = vec4(vertColor, 1.0f);\n"
                "}\n"
        },
        .fragmentShader =
        {
            .source =
                "#version 330 core\n"
                "in vec4 fragColor;\n"
                "out vec4 finalColor;\n"
                "void main()\n"
                "{\n"
                "    finalColor = fragColor;\n"
                "}\n"
        }
    };

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

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 transform;
    transform = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    transform = glm::rotate(transform, (float)glfwGetTime(),
        glm::vec3(0.0f, 1.0f, 0.0f));

    glUseProgram(m_shader.GetHandle());
    glUniformMatrix4fv(m_shader.GetUniformIndex("vertTransform"),
        1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(m_vertexArray.GetHandle());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.GetHandle());
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

    OPENGL_CHECK_ERRORS();
}

void Application::OnKeyboardKey(KeyboardKeyEvent event)
{
    if(event.key == GLFW_KEY_ESCAPE && event.action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}
