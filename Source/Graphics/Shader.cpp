#include "Precompiled.hpp"
#include "Shader.hpp"

Graphics::Shader::~Shader()
{
    if(m_handle != 0)
    {
        glDeleteProgram(m_handle);
        OPENGL_CHECK_ERRORS();
    }
}

bool Graphics::Shader::Setup(LoadFromFiles& params)
{
    fs::path* inputParamsMapping[] =
    {
        &params.vertexShader,
        &params.fragmentShader,
    };

    LoadFromSources setupParams;
    LoadFromSources::Shader* outputParamsMapping[] =
    {
        &setupParams.vertexShader,
        &setupParams.fragmentShader,
    };

    static_assert(std::size(inputParamsMapping) == std::size(outputParamsMapping));

    for(int i = 0; i < std::size(outputParamsMapping); ++i)
    {
        fs::path& path = *inputParamsMapping[i];
        if(path.empty())
            continue;

        auto result = Utility::ReadTextFile(path);
        if(!result.has_value())
        {
            LOG_ERROR("Failed to read shader source from \"{}\" file", path);
            return false;
        }

        LoadFromSources::Shader& shaderSource = *outputParamsMapping[i];
        shaderSource.source = std::move(result.value());
        shaderSource.path = std::move(path);
    }

    return Setup(setupParams);
}

bool Graphics::Shader::Setup(LoadFromSources& params)
{
    OPENGL_CHECK_ERRORS_SCOPED();
    ASSERT(m_handle == OpenGL::InvalidHandle);

    struct ParamMapping
    {
        LoadFromSources::Shader& shader;
        GLenum shaderType;
    };

    ParamMapping paramsMapping[] =
    {
        { params.vertexShader, GL_VERTEX_SHADER },
        { params.fragmentShader, GL_FRAGMENT_SHADER },
    };

    constexpr int ShaderTypeCount = std::size(paramsMapping);

    GLuint shaderObjects[ShaderTypeCount] = {};
    SCOPE_GUARD([&shaderObjects]()
    {
        for(GLuint shaderObject : shaderObjects)
        {
            glDeleteShader(shaderObject);
        }
    });

    bool shaderObjectCompiled = false;
    for(int i = 0; i < ShaderTypeCount; ++i)
    {
        const ParamMapping& param = paramsMapping[i];
        if(param.shader.source.empty())
            continue;

        GLuint& shaderObject = shaderObjects[i];
        shaderObject = glCreateShader(param.shaderType);
        const char* shaderSourcePtr = param.shader.source.c_str();
        glShaderSource(shaderObject, 1, &shaderSourcePtr, nullptr);
        glCompileShader(shaderObject);
    
        GLint compileLogLength = 0;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compileLogLength);
        if(compileLogLength > 1)
        {
            std::vector<char> compileLogBuffer(compileLogLength);
            glGetShaderInfoLog(shaderObject, compileLogLength, nullptr, compileLogBuffer.data());
            if(!param.shader.path.empty())
            {
                LOG_INFO("Shader object compile log for \"{}\" file:\n{}",
                    param.shader.path, compileLogBuffer.data());
            }
            else
            {
                LOG_INFO("Shader object compile log:\n{}", compileLogBuffer.data());
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

    LOG_TRACE("Created shader program");

    return true;
}

GLint Graphics::Shader::GetAttributeIndex(const char* name) const
{
    GLint location = glGetAttribLocation(m_handle, name);
    OPENGL_CHECK_ERRORS();
    return location;
}

GLint Graphics::Shader::GetUniformIndex(const char* name) const
{
    GLint location = glGetUniformLocation(m_handle, name);
    OPENGL_CHECK_ERRORS();
    return location;
}
