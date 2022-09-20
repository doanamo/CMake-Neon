#include "Precompiled.hpp"
#include "Texture.hpp"
#include "OpenGL.hpp"
#include "System/Image.hpp"

// #Todo:
// - Support texture compression

namespace Graphics
{
    Texture::~Texture() noexcept
    {
        if(m_handle != 0)
        {
            glDeleteTextures(1, &m_handle);
            OPENGL_CHECK_ERRORS();

            LOG_TRACE("Destroyed texture");
        }
    }

    bool Texture::Setup(const SetupFromImage& params)
    {
        OPENGL_CHECK_ERRORS_SCOPED();

        const System::Image& image = params.image;
        uint8_t channels = image.GetChannels();

        GLenum format;
        switch(channels)
        {
        case 1:
            format = GL_RED;
            break;

        case 2:
            format = GL_RG;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;

        default:
            ASSERT(false, "Unsupported image channel count");
            return false;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, channels);
        SCOPE_GUARD([]
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        });

        SetupFromData setupParams =
        {
            .data = image.GetData(),
            .format = format,
            .width = image.GetWidth(),
            .height = image.GetHeight(),
            .mipmaps = params.mipmaps,
        };

        return Setup(setupParams);
    }

    bool Texture::Setup(const SetupFromData& params)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle == 0);

        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);
        SCOPE_GUARD([]
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        });

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, params.format, params.width, params.height,
            0, params.format, GL_UNSIGNED_BYTE, params.data);

        if(params.mipmaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        m_format = params.format;
        m_width = params.width;
        m_height = params.height;

        LOG_TRACE("Created texture");
        return true;
    }
} // namespace Graphics
