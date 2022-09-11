#pragma once

namespace OpenGL
{
    void PrintExtensions();

    const char* GetErrorName(GLuint error);
    bool CheckErrors();
}

#ifdef CONFIG_DEBUG
    #define OPENGL_CHECK_ERRORS() OpenGL::CheckErrors()
    #define OPENGL_CHECK_ERRORS_SCOPED() \
        SCOPE_GUARD([]() { OPENGL_CHECK_ERRORS(); })
#else
    #define OPENGL_CHECK_ERRORS()
    #define OPENGL_CHECK_ERRORS_SCOPED()
#endif
