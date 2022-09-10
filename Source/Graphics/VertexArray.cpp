#include "Precompiled.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"

namespace Graphics
{
    VertexArray::~VertexArray() noexcept
    {
        if(m_handle != 0)
        {
            glDeleteVertexArrays(1, &m_handle);
            OPENGL_CHECK_ERRORS();

            LOG_TRACE("Destroyed vertex array");
        }
    }

    bool VertexArray::Setup(const SetupAttribute* attributes, uint32_t count)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle == 0);

        glGenVertexArrays(1, &m_handle);
        glBindVertexArray(m_handle);

        ASSERT(attributes && count > 0);
        for(int i = 0; i < count; ++i)
        {
            const SetupAttribute& attribute = attributes[i];

            glBindBuffer(attribute.buffer.GetType(), attribute.buffer.GetHandle());
            glEnableVertexAttribArray(attribute.location);
            glVertexAttribPointer(
                attribute.location,
                attribute.count,
                attribute.type,
                GL_FALSE,
                attribute.stride,
                (void*)attribute.offset
            );
            glBindBuffer(attribute.buffer.GetType(), 0);
        }

        glBindVertexArray(0);

        LOG_TRACE("Created vertex array");
        return true;
    }
} // namespace Graphics
