#pragma once

#include "OpenGL.hpp"

namespace Graphics
{
    class Buffer final
    {
    public:
        Buffer() = default;
        ~Buffer() noexcept;

        Buffer(Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        struct SetupFromParams
        {
            GLenum type = 0;
            GLenum usage = GL_STATIC_DRAW;
            uint32_t elementSize = 0;
            uint32_t elementCount = 0;
            const void* data = nullptr;
        };

        bool Setup(const SetupFromParams& params);
        void Update(uint32_t elementIndex, uint32_t elementCount, const void* data);
        void Resize(uint32_t elementCount, const void* data);

        GLuint GetHandle() const
        {
            ASSERT(m_handle != 0);
            return m_handle;
        }

        GLenum GetType() const
        {
            ASSERT(m_handle != 0);
            return m_type;
        }

        std::size_t GetElementSize() const
        {
            ASSERT(m_handle != 0);
            return m_elementSize;
        }
        
        std::size_t GetElementCount() const
        {
            ASSERT(m_handle != 0);
            return m_elementCount;
        }

        std::size_t GetSize() const
        {
            ASSERT(m_handle != 0);
            return m_elementSize * m_elementCount;
        }

        GLenum GetIndexEnum() const;

    private:
        GLuint m_handle = 0;
        GLenum m_type = 0;
        GLenum m_usage = 0;
        uint32_t m_elementSize = 0;
        uint32_t m_elementCount = 0;
    };
} // namespace Graphics
