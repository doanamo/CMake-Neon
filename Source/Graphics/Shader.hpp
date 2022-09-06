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
            fs::path vertexShaderPath;
            fs::path fragmentShaderPath;
        };

        bool Setup(LoadFromFiles& params);

        struct LoadFromSources
        {
            struct Shader
            {
                std::string source;
                fs::path path;
            };

            Shader vertexShader;
            Shader fragmentShader;
        };

        bool Setup(LoadFromSources& params);

        GLint GetAttributeIndex(const char* name) const;
        GLint GetUniformIndex(const char* name) const;

    private:
        GLuint m_handle = OpenGL::InvalidHandle;
    };
} // namespace Graphics
