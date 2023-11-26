#include "Renderer.h"
#include <iostream>

void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

bool GLLogCall(const char *function, const char *file, int line) {
  bool isError = false;

  while (GLenum error = glGetError()) {
    std::cout << "Error " << error << " in function " << function << " of file "
              << file << " in line " << line << std::endl;
    isError = true;
  }

  return isError;
}

void Renderer::Clear() const { glClear(GL_COLOR_BUFFER_BIT); }

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib,
                    const Shader &shader) const {

  /* Use the shader and set it's uniform color */
  shader.Bind();
  /* Select the vertex array buffer */
  va.Bind();
  /* Select the index buffer */
  ib.Bind();

  /* Draw call */
  GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
