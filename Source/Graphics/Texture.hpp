#pragma once

namespace System
{
    class Image;
}

namespace Graphics
{
    class Texture final
    {
    public:
        Texture() = default;
        ~Texture() noexcept;

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        struct SetupFromImage
        {
            const System::Image& image;
            bool mipmaps = true;
        };

        bool Setup(const SetupFromImage& params);

        struct SetupFromData
        {
            const uint8_t* data = nullptr;
            GLenum format = 0;
            uint32_t width = 0;
            uint32_t height = 0;
            bool mipmaps = true;
        };

        bool Setup(const SetupFromData& params);

        GLuint GetHandle() const
        {
            ASSERT(m_handle != 0);
            return m_handle;
        }

        GLenum GetFormat() const
        {
            ASSERT(m_format != 0);
            return m_format;
        }
        
        uint32_t GetWidth() const
        {
            ASSERT(m_width != 0);
            return m_width;
        }

        uint32_t GetHeight() const
        {
            ASSERT(m_height != 0);
            return m_height;
        }

    private:
        GLuint m_handle = 0;
        GLenum m_format = 0;
        uint32_t m_width = 0;
        uint32_t m_height = 0;
    };
} // namespace Graphics
