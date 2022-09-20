#pragma once

namespace System
{
    class Image final
    {
    public:
        Image() = default;
        ~Image() noexcept;

        Image(const Image&) = default;
        Image& operator=(const Image&) = default;

        struct SetupFromFile
        {
            fs::path path;
        };

        bool Setup(const SetupFromFile& params);

        const uint8_t* GetData() const
        {
            ASSERT(!m_data.empty());
            return m_data.data();
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

        uint8_t GetChannels() const
        {
            ASSERT(m_channels != 0);
            return m_channels;
        }

    private:
        std::vector<uint8_t> m_data;
        uint32_t m_width = 0;
        uint32_t m_height = 0;
        uint8_t m_channels = 0;
    };
} // namespace Image
