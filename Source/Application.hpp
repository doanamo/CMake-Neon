#pragma once

#include "Graphics/OpenGL.hpp"
#include "Graphics/Buffer.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"

class Application final
{
public:
    explicit Application(GLFWwindow* window);
    ~Application() = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool Setup();
    void Update(float deltaTime);
    void Render(float alphaTime);

    struct KeyboardKeyEvent
    {
        int key{};
        int scancode{};
        int action{};
        int mods{};
    };

    void OnKeyboardKey(KeyboardKeyEvent event);

private:
    GLFWwindow* m_window = nullptr;

    Graphics::Buffer m_vertexBuffer;
    Graphics::Buffer m_indexBuffer;
    Graphics::VertexArray m_vertexArray;
    Graphics::Texture m_texture;
    Graphics::Shader m_shader;
};
