#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <GL/glext.h>
#include <iostream>

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::cout << "Error" << std::endl;
    return -1;
  }

  /* Buffer here */
  unsigned int buffer;

  /* Generate a buffer an select it */
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  float positions[6] = {
    -0.5f, -0.5f, 
    0.0f, 0.5f,
    0.5f, -0.5f
  };

  /* Attribute data to the buffer */
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw triangle
    glBegin(GL_TRIANGLES);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glEnd();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
