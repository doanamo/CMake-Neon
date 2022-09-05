#pragma once

#include "OpenGL.hpp"

namespace Graphics
{
    enum struct ShaderType
    {
        VertexShader,
        FragmentShader,
        Count,
    };

    constexpr auto ShaderTypeCount = static_cast<size_t>(ShaderType::Count);

    ShaderType GetShaderType(int index);
    size_t GetShaderTypeIndex(ShaderType type);
    GLenum GetShaderTypeEnum(ShaderType type);

    class Shader final
    {
    public:
        Shader() = default;
        ~Shader();

        Shader(Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        struct LoadFromFiles
        {
            fs::path paths[ShaderTypeCount] = {};
            void SetPath(ShaderType type, fs::path path);
        };

        bool Setup(LoadFromFiles& params);

        struct LoadFromSources
        {
            struct SourceEntry
            {
                std::string source;
                fs::path path;
            };

            SourceEntry sources[ShaderTypeCount] = {};
            void SetSource(ShaderType type, std::string source, fs::path path = {});
        };

        bool Setup(LoadFromSources& params);

    private:
        GLuint m_handle = OpenGL::InvalidHandle;
    };
} // namespace Graphics
