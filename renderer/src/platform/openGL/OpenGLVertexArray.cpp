#include "pch.h"

#include "OpenGLVertexArray.h"

namespace Bottleneck {
    static GLenum shaderDataTypeToOpenGLBaseType(const ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
            default:
                assert(false);
        }
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        glGenVertexArrays(1, &m_rendererID);
        glBindVertexArray(m_rendererID);
        //glBindVertexArray(0);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        glDeleteVertexArrays(1, &m_rendererID);
    }

    void OpenGLVertexArray::bind() const {
        glBindVertexArray(m_rendererID);
    }

    void OpenGLVertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
        assert(!vertexBuffer->getLayout().getElements().empty());

        glBindVertexArray(m_rendererID);
        vertexBuffer->bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->getLayout();
        for (const auto& element: layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.getComponentCount(),
                                  shaderDataTypeToOpenGLBaseType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  layout.getStride(),
                                  reinterpret_cast<const void*>(element.offset));
            index += 1;
        }

        m_vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
        glBindVertexArray(m_rendererID);
        indexBuffer->bind();
        m_indexBuffer = indexBuffer;
    }
}
