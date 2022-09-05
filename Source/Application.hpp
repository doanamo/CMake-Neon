#pragma once

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

    Graphics::Shader m_shader;
};
