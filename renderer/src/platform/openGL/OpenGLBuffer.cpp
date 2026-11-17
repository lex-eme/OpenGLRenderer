#include "pch.h"

#include "OpenGLBuffer.h"

namespace Bottleneck {
    ////////////////////////////////////////////////////////////
    // VertexBuffer ////////////////////////////////////////////
    ////////////////////////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const uint32_t size) {
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLVertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setLayout(const BufferLayout& layout) {
        m_layout = layout;
    }

    ////////////////////////////////////////////////////////////
    // IndexBuffer /////////////////////////////////////////////
    ////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count)
        : m_count(count) {
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLIndexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLIndexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
