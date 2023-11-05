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
