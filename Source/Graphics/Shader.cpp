#include "Precompiled.hpp"
#include "Shader.hpp"

namespace Graphics
{
    Shader::~Shader() noexcept
    {
        if(m_handle != 0)
        {
            glDeleteProgram(m_handle);
            OPENGL_CHECK_ERRORS();

            LOG_TRACE("Destroyed shader");
        }
    }

    bool Shader::Setup(SetupFromFiles& params)
    {
        fs::path* const shaderPaths[] =
        {
            &params.vertexShader,
            &params.fragmentShader,
        };

        SetupFromSources setupFromSources;
        SetupFromSources::ShaderSource* const shaderSources[] =
        {
            &setupFromSources.vertexShader,
            &setupFromSources.fragmentShader,
        };

        static_assert(std::size(shaderPaths) == std::size(shaderSources));
        for(int i = 0; i < std::size(shaderSources); ++i)
        {
            fs::path& path = *shaderPaths[i];
            if(path.empty())
                continue;

            auto result = Utility::ReadTextFile(path);
            if(!result.has_value())
            {
                LOG_ERROR("Failed to read shader source from \"{}\" file", path);
                return false;
            }

            auto& shaderSource = *shaderSources[i];
            shaderSource.source = std::move(result.value());
            shaderSource.path = std::move(path);
        }

        return Setup(setupFromSources);
    }

    bool Shader::Setup(SetupFromSources& params)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle == OpenGL::InvalidHandle);

        struct ShaderSource
        {
            std::string& source;
            fs::path& path;
            GLenum type;
        };

        ShaderSource const shaderSources[] =
        {
            {
                params.vertexShader.source,
                params.vertexShader.path,
                GL_VERTEX_SHADER
            },
            {
                params.fragmentShader.source,
                params.fragmentShader.path,
                GL_FRAGMENT_SHADER
            },
        };

        GLuint shaderObjects[std::size(shaderSources)] = {};
        SCOPE_GUARD([&shaderObjects]()
        {
            for(GLuint shaderObject : shaderObjects)
            {
                glDeleteShader(shaderObject);
            }
        });

        bool shaderObjectCompiled = false;
        for(int i = 0; i < std::size(shaderSources); ++i)
        {
            const ShaderSource& shader = shaderSources[i];
            if(shader.source.empty())
                continue;

            GLuint& shaderObject = shaderObjects[i];
            shaderObject = glCreateShader(shader.type);
            const char* shaderSourcePtr = shader.source.c_str();
            glShaderSource(shaderObject, 1, &shaderSourcePtr, nullptr);
            glCompileShader(shaderObject);

            GLint compileLogLength = 0;
            glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compileLogLength);
            if(compileLogLength > 1)
            {
                std::vector<char> compileLogBuffer(compileLogLength);
                glGetShaderInfoLog(shaderObject, compileLogLength,
                    nullptr, compileLogBuffer.data());
                if(!shader.path.empty())
                {
                    LOG_INFO("Shader object compile log for \"{}\" file:\n{}",
                        shader.path, compileLogBuffer.data());
                }
                else
                {
                    LOG_INFO("Shader object compile log:\n{}",
                        compileLogBuffer.data());
                }
            }

            GLint compileStatus = GL_FALSE;
            glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileStatus);
            if(compileStatus == GL_FALSE)
            {
                LOG_ERROR("Failed to compile shader object due to errors");
                return false;
            }

            shaderObjectCompiled = true;
        }

        if(!shaderObjectCompiled)
        {
            LOG_ERROR("Failed to compile any shader objects");
            return false;
        }

        m_handle = glCreateProgram();
        for(GLuint shaderObject : shaderObjects)
        {
            if(shaderObject != OpenGL::InvalidHandle)
            {
                glAttachShader(m_handle, shaderObject);
            }
        }

        glLinkProgram(m_handle);
        for(GLuint shaderObject : shaderObjects)
        {
            if(shaderObject != OpenGL::InvalidHandle)
            {
                glDetachShader(m_handle, shaderObject);
            }
        }

        GLint linkLogLength = 0;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &linkLogLength);
        if(linkLogLength > 1)
        {
            std::vector<char> linkLogBuffer(linkLogLength);
            glGetProgramInfoLog(m_handle, linkLogLength, nullptr, linkLogBuffer.data());
            LOG_INFO("Shader program link log:\n{}", linkLogBuffer.data());
        }

        GLint linkStatus = GL_FALSE;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &linkStatus);
        if(linkStatus == GL_FALSE)
        {
            LOG_ERROR("Failed to link shader program");
            return false;
        }

        LOG_TRACE("Created shader");
        return true;
    }
} // namespace Graphics
