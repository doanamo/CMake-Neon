#include "Precompiled.hpp"
#include "Application.hpp"

void GLFWErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW Error: {} [{}]", description, error);
}

void GLFWKeyboardKeyCallback(GLFWwindow* window,
    int key, int scancode, int action, int mods)
{
    auto* app = reinterpret_cast<Application*>(
        glfwGetWindowUserPointer(window));

    if(app != nullptr)
    {
        app->OnKeyboardKey(Application::KeyboardKeyEvent
            {
                .key = key,
                .scancode = scancode,
                .action = action,
                .mods = mods
            });
    }
}

int main()
{
    Logger::Setup();

    // Initialize GLFW library
    if(!glfwInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
        return 1;
    }

    SCOPE_GUARD([]()
    {
        glfwTerminate();
    });

    glfwSetErrorCallback(GLFWErrorCallback);

    // Create GLFW window
    const char* title = "Game";
    const int width = 1024;
    const int height = 576;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(
        width, height, title, nullptr, nullptr);

    if(window == nullptr)
    {
        LOG_ERROR("Failed to create window");
        return 1;
    }

    SCOPE_GUARD([&window]()
    {
        glfwDestroyWindow(window);
    });

    glfwSetKeyCallback(window, GLFWKeyboardKeyCallback);

    LOG_INFO("Created window");

    // Load OpenGL context
    glfwMakeContextCurrent(window);

    int contextVersion = gladLoadGL(glfwGetProcAddress);
    if(contextVersion == 0)
    {
        LOG_ERROR("Failed to load OpenGL context");
        return 1;
    }

    LOG_INFO("Loaded OpenGL {}.{} context",
        GLAD_VERSION_MAJOR(contextVersion),
        GLAD_VERSION_MINOR(contextVersion));

    // Create application
    Application app(window);
    if(!app.Setup())
    {
        LOG_ERROR("Failed to initialize application");
        return -1;
    }

    glfwSetWindowUserPointer(window, &app);

    // Run main loop
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        app.Update(1.0F);
        app.Render(1.0F);

        glfwSwapBuffers(window);
    }

    // Shutdown application
    LOG_INFO("Shutting down...");
    return 0;
}
