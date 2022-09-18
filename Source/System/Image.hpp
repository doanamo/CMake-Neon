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

    private:
        std::vector<uint8_t> m_data;
        uint32_t m_width = 0;
        uint32_t m_height = 0;
        uint32_t m_channels = 0;
    };
} // namespace Image
