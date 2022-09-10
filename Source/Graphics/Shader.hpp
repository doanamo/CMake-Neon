#pragma once

#include "OpenGL.hpp"

namespace Graphics
{
    class Shader final
    {
    public:
        Shader() = default;
        ~Shader() noexcept;

        Shader(Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        struct SetupFromFiles
        {
            fs::path vertexShader;
            fs::path fragmentShader;
        };

        bool Setup(SetupFromFiles& params);

        struct SetupFromSources
        {
            struct ShaderSource
            {
                std::string source;
                fs::path path;
            };

            ShaderSource vertexShader;
            ShaderSource fragmentShader;
        };

        bool Setup(SetupFromSources& params);

        GLuint GetHandle() const
        {
            ASSERT(m_handle != 0);
            return m_handle;
        }
        
        GLint GetAttributeIndex(const char* name) const
        {
            GLint location = glGetAttribLocation(m_handle, name);
            OPENGL_CHECK_ERRORS();
            return location;
        }

        GLint GetUniformIndex(const char* name) const
        {
            GLint location = glGetUniformLocation(m_handle, name);
            OPENGL_CHECK_ERRORS();
            return location;
        }

    private:
        GLuint m_handle = 0;
    };
} // namespace Graphics
