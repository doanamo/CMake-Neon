#pragma once

namespace OpenGL
{
    constexpr GLuint InvalidHandle = 0;
    constexpr GLenum InvalidEnum = 0;

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
