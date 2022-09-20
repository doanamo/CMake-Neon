#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Image.hpp"

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
        glm::vec2 texture;
    };

    Vertex vertices[] =
    {
        { { -0.6f,  0.6f, 0.0f }, { 0.0f, 1.0f } },
        { {  0.6f,  0.6f, 0.0f }, { 1.0f, 1.0f } },
        { {  0.6f, -0.6f, 0.0f }, { 1.0f, 0.0f } },
        { { -0.6f, -0.6f, 0.0f }, { 0.0f, 0.0f } }
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
        0, 1, 2,
        2, 3, 0
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
            .count = 2,
            .stride = sizeof(Vertex),
            .offset = offsetof(Vertex, texture)
        }
    };

    Graphics::VertexArray::SetupFromParams vertexArrayParams =
    {
        .attributes = vertexAttributes,
        .attributeCount = std::size(vertexAttributes),
        .indexBuffer = &m_indexBuffer
    };

    if(!m_vertexArray.Setup(vertexArrayParams))
        return false;

    // Shader
    Graphics::Shader::SetupFromFiles shaderParams =
    {
        .vertexShader = "Data/Shaders/vertex_texture_vs.glsl",
        .fragmentShader = "Data/Shaders/vertex_texture_fs.glsl"
    };

    if(!m_shader.Setup(shaderParams))
        return false;

    // Texture
    System::Image::SetupFromFile imageParams =
    {
        .path = "Data/Textures/puffy.png"
    };

    System::Image image;
    if(!image.Setup(imageParams))
        return false;

    Graphics::Texture::SetupFromImage textureParams =
    {
        .image = image
    };

    if(!m_texture.Setup(textureParams))
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

    glBindTexture(GL_TEXTURE_2D, m_texture.GetHandle());
    glBindVertexArray(m_vertexArray.GetHandle());
    glDrawElements(GL_TRIANGLES, m_indexBuffer.GetElementCount(),
        m_indexBuffer.GetIndexEnum(), 0);

    OPENGL_CHECK_ERRORS();
}

void Application::OnKeyboardKey(KeyboardKeyEvent event)
{
    if(event.key == GLFW_KEY_ESCAPE && event.action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}
