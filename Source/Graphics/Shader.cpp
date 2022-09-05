#include "Precompiled.hpp"
#include "Shader.hpp"

Graphics::ShaderType Graphics::GetShaderType(int index)
{
    ASSERT(index < ShaderTypeCount, "Index out of range!");
    return static_cast<ShaderType>(index);
}

size_t Graphics::GetShaderTypeIndex(ShaderType type)
{
    ASSERT(type < ShaderType::Count, "Invalid type!");
    return static_cast<size_t>(type);
}

GLenum Graphics::GetShaderTypeEnum(ShaderType type)
{
    switch(type)
    {
    case ShaderType::VertexShader:
        return GL_VERTEX_SHADER;

    case ShaderType::FragmentShader:
        return GL_FRAGMENT_SHADER;

    default:
        ASSERT(false, "Invalid shader type!");
        return GL_INVALID_ENUM;
    }
}

void Graphics::Shader::LoadFromFiles::SetPath(ShaderType type, fs::path path)
{
    paths[GetShaderTypeIndex(type)] = std::move(path);
}

void Graphics::Shader::LoadFromSources::SetSource(ShaderType type, std::string source, fs::path path)
{
    sources[GetShaderTypeIndex(type)] =
    {
        .source = std::move(source),
        .path = std::move(path),
    };
}

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
    LoadFromSources setupParams;
    for(int i = 0; i < ShaderTypeCount; ++i)
    {
        fs::path& path = params.paths[i];
        if(path.empty())
            continue;

        auto result = Utility::ReadTextFile(path);
        if(!result.has_value())
        {
            LOG_ERROR("Failed to read shader source from \"{}\" file", path);
            return false;
        }

        setupParams.sources[i] =
        {
            .source = std::move(result.value()),
            .path = std::move(path),
        };
    }

    return Setup(setupParams);
}

bool Graphics::Shader::Setup(LoadFromSources& params)
{
    OPENGL_CHECK_ERRORS_SCOPED();
    ASSERT(m_handle == OpenGL::InvalidHandle);

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
        const ShaderType shaderType = GetShaderType(i);
        const std::string& shaderSource = params.sources[i].source;
        const fs::path& sourcePath = params.sources[i].path;
        if(shaderSource.empty())
            continue;

        GLuint& shaderObject = shaderObjects[i];
        shaderObject = glCreateShader(GetShaderTypeEnum(shaderType));
        const char* shaderSourcePtr = shaderSource.c_str();
        glShaderSource(shaderObject, 1, &shaderSourcePtr, nullptr);
        glCompileShader(shaderObject);
    
        GLint compileLogLength = 0;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compileLogLength);
        if(compileLogLength > 1)
        {
            std::vector<char> compileLogBuffer(compileLogLength);
            glGetShaderInfoLog(shaderObject, compileLogLength, nullptr, compileLogBuffer.data());
            if(!sourcePath.empty())
            {
                LOG_INFO("Shader object compile log for \"{}\" file:\n{}",
                    sourcePath, compileLogBuffer.data());
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
