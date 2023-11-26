#include "VertexArray.h"

VertexArray::VertexArray() {
  GLCall(glGenVertexArrays(1, &m_RendererID));
  GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
  GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb,
                            const VertexBufferLayout &layout) {
  Bind();
  vb.Bind();

  const auto &elements = layout.GetElements();
  unsigned int offset = 0;

  for (unsigned int i = 0; i < elements.size(); ++i) {
    const auto &element = elements[i];
    /* Describe the layout of the attributes in the vertex and enables it*/
    GLCall(glEnableVertexAttribArray(i));
    /* Links buffer with vao at index 0 */
    GLCall(glVertexAttribPointer(i, element.count, element.type,
                                 element.normalized, layout.GetStride(),
                                 (const void *)(unsigned long long)offset));
    offset += element.count * VertexBufferElement::GetTypeSize(element.type);
  }
}

void VertexArray::Bind() const {
  GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
  GLCall(glBindVertexArray(0));
}
