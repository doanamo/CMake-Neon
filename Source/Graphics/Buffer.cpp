#include "Precompiled.hpp"
#include "Buffer.hpp"

namespace Graphics
{
    Buffer::~Buffer() noexcept
    {
        if(m_handle != 0)
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
        ASSERT(m_handle == 0);

        GLsizeiptr bufferSize = params.elementSize * params.elementCount;
        ASSERT(bufferSize > 0, "Cannot create empty buffer");

        glGenBuffers(1, &m_handle);
        glBindBuffer(params.type, m_handle);
        glBufferData(params.type, bufferSize, params.data, params.usage);
        glBindBuffer(params.type, 0);

        m_type = params.type;
        m_usage = params.usage;
        m_elementSize = params.elementSize;
        m_elementCount = params.elementCount;

        LOG_TRACE("Created buffer ({} bytes)", bufferSize);
        return true;
    }

    void Buffer::Update(uint32_t elementIndex, uint32_t elementCount, const void* data)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle != 0);

        uint64_t byteOffset = m_elementSize * elementIndex;
        uint64_t byteSize = m_elementSize * elementCount;

        ASSERT(byteOffset + byteSize <= GetSize(), "Uploading data past buffer size");

        glBindBuffer(m_type, m_handle);
        glBufferSubData(m_type, byteOffset, byteSize, data);
        glBindBuffer(m_type, 0);
    }

    void Buffer::Resize(uint32_t elementCount, const void* data)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle != 0);

        uint64_t bufferSize = m_elementSize * elementCount;
        ASSERT(bufferSize > 0, "Cannot create empty buffer");

        glBindBuffer(m_type, m_handle);
        glBufferData(m_type, bufferSize, data, m_usage);
        glBindBuffer(m_type, 0);

        LOG_TRACE("Resized buffer ({} -> {} bytes)", GetSize(), bufferSize);

        m_elementCount = elementCount;
    }

    GLenum Buffer::GetIndexEnum() const
    {
        ASSERT(m_handle != 0 && m_type == GL_ELEMENT_ARRAY_BUFFER);
        
        switch(m_elementSize)
        {
        case 1: return GL_UNSIGNED_BYTE;
        case 2: return GL_UNSIGNED_SHORT;
        case 4: return GL_UNSIGNED_INT;
        }

        ASSERT(false, "Invalid index size");
        return 0;
    }

} // namespace Graphics
