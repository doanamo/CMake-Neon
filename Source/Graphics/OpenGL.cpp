#include "Precompiled.hpp"
#include "OpenGL.hpp"

const char* OpenGL::GetErrorName(GLuint error)
{
    switch(error)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";

    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";

    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";

    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";

    default:
        return "Unknown";
    }
}

bool OpenGL::CheckErrors()
{
    bool foundErrors = false;

    while(true)
    {
        GLuint error = glGetError();
        if(error == GL_NO_ERROR)
            break;

        LOG_ERROR("OpenGL Error: {} {:#06x}", GetErrorName(error), error);
        foundErrors = true;
    }

    ASSERT(!foundErrors, "Detected OpenGL errors");
    return false;
}
