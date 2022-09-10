#include "Precompiled.hpp"
#include "Buffer.hpp"

namespace Graphics
{
    Buffer::~Buffer() noexcept
    {
        if(m_handle != OpenGL::InvalidHandle)
        {
            glDeleteBuffers(1, &m_handle);
            OPENGL_CHECK_ERRORS();

            LOG_TRACE("Destroyed buffer ({} bytes)",
                m_elementSize * m_elementCount);
        }
    }

    bool Buffer::Setup(const SetupFromParams& params)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle == OpenGL::InvalidHandle);

        GLsizeiptr bufferSize = params.elementSize * params.elementCount;
        ASSERT(bufferSize > 0, "Cannot create empty buffer!");

        glGenBuffers(1, &m_handle);
        glBindBuffer(params.type, m_handle);
        glBufferData(params.type, bufferSize, params.data, params.usage);

        m_type = params.type;
        m_usage = params.usage;
        m_elementSize = params.elementSize;
        m_elementCount = params.elementCount;

        LOG_TRACE("Created buffer ({} bytes)", bufferSize);
        return true;
    }

    void Buffer::Update(uint64_t elementIndex, uint64_t elementCount, const void* data)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle != OpenGL::InvalidHandle);

        uint64_t offset = m_elementSize * elementIndex;
        uint64_t size = m_elementSize * elementCount;

        ASSERT(offset + size <= GetSize(), "Uploading data past buffer size!");

        glBindBuffer(m_type, m_handle);
        glBufferSubData(m_type, offset, size, data);
    }

    void Buffer::Resize(uint64_t elementCount, const void* data)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle != OpenGL::InvalidHandle);

        uint64_t bufferSize = m_elementSize * elementCount;
        ASSERT(bufferSize > 0, "Cannot create empty buffer!");

        glBindBuffer(m_type, m_handle);
        glBufferData(m_type, bufferSize, data, m_usage);

        LOG_TRACE("Resized buffer ({} -> {} bytes)", GetSize(), bufferSize);

        m_elementCount = elementCount;
    }
} // namespace Graphics
