#pragma once

#include "OpenGL.hpp"

namespace Graphics
{
    class Buffer;

    class VertexArray final
    {
    public:
        VertexArray() = default;
        ~VertexArray() noexcept;

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        struct SetupAttribute
        {
            const Buffer& buffer;
            GLenum type = 0;
            uint32_t location = 0;
            uint32_t count = 0;
            uint32_t stride = 0;
            uint64_t offset = 0;
        };

        struct SetupFromParams
        {
            const SetupAttribute* attributes = nullptr;
            uint32_t attributeCount = 0;
            const Buffer* indexBuffer = nullptr;
        };

        bool Setup(const SetupFromParams& params);

        GLuint GetHandle() const
        {
            ASSERT(m_handle != 0);
            return m_handle;
        }

    private:
        GLuint m_handle = 0;
    };
} // namespace Graphics
