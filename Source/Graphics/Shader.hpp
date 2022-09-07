#pragma once

#include "OpenGL.hpp"

namespace Graphics
{
    class Shader final
    {
    public:
        Shader() = default;
        ~Shader();

        Shader(Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        struct LoadFromFiles
        {
            fs::path vertexShader;
            fs::path fragmentShader;
        };

        bool Setup(LoadFromFiles& params);

        struct LoadFromSources
        {
            struct ShaderSource
            {
                std::string source;
                fs::path path;
            };

            ShaderSource vertexShader;
            ShaderSource fragmentShader;
        };

        bool Setup(LoadFromSources& params);

        GLint GetAttributeIndex(const char* name) const;
        GLint GetUniformIndex(const char* name) const;

    private:
        GLuint m_handle = OpenGL::InvalidHandle;
    };
} // namespace Graphics
