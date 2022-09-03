#include "Precompiled.hpp"

int main()
{
    // Initialize GLFW library
    if(!glfwInit())
    {
        return 1;
    }

    glfwTerminate();
    return 0;
}
