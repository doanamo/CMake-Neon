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

    bool VertexArray::Setup(const SetupFromParams& params)
    {
        OPENGL_CHECK_ERRORS_SCOPED();
        ASSERT(m_handle == 0);

        glGenVertexArrays(1, &m_handle);
        glBindVertexArray(m_handle);

        if(params.indexBuffer)
        {
            ASSERT(params.indexBuffer->GetType() == GL_ELEMENT_ARRAY_BUFFER);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, params.indexBuffer->GetHandle());
        }

        ASSERT(params.attributes && params.attributeCount > 0);
        for(int i = 0; i < params.attributeCount; ++i)
        {
            const SetupAttribute& attribute = params.attributes[i];

            ASSERT(attribute.buffer.GetType() == GL_ARRAY_BUFFER);
            glBindBuffer(GL_ARRAY_BUFFER, attribute.buffer.GetHandle());

            glEnableVertexAttribArray(attribute.location);
            glVertexAttribPointer(
                attribute.location,
                attribute.count,
                attribute.type,
                GL_FALSE,
                attribute.stride,
                (void*)attribute.offset
            );

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        LOG_TRACE("Created vertex array");
        return true;
    }
} // namespace Graphics
