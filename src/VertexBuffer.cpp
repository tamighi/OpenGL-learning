#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
  /* Create the buffer */
  GLCall(glGenBuffers(1, &m_RendererID));
  /* Bind buffer */
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  /* Attribute data to the buffer */
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

void VertexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
