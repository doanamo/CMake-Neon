#pragma once

namespace Graphics
{
    class Texture final
    {
    public:
        Texture() = default;
        ~Texture() = default;

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

    private:
        GLuint m_handle = 0;
    };
} // namespace Graphics
