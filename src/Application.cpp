#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    std::cout << "Error" << std::endl;
    return -1;
  }

  float positions[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  /* Vertex array buffer */
  VertexArray va;
  /* Array buffer */
  VertexBuffer vb(positions, 4 * 2 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);

  va.AddBuffer(vb, layout);

  /* Index buffer */
  IndexBuffer ib(indices, 6);

  /* Create shaders */
  Shader shader("res/shaders/Basic.vertex.shader",
                "res/shaders/Basic.fragment.shader");

  /* Unbind buffers */
  va.Unbind();
  vb.Unbind();
  shader.Unbind();
  ib.Unbind();

  Renderer renderer;

  float r = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    renderer.Clear();

    /* Use the shader and set it's uniform color */
    shader.Bind();
    shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    renderer.Draw(va, ib, shader);

    if (r > 1.0f || r < 0.0f)
      increment *= -1;
    r += increment;

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
