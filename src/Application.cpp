#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

static std::string ParseShader(const std::string &filepath) {
  std::ifstream stream(filepath);
  std::stringstream ss;
  std::string line;

  while (std::getline(stream, line)) {
    ss << line << '\n';
  }

  return ss.str();
}

static unsigned int CompileShader(unsigned int type,
                                  const std::string &source) {
  GLCall(unsigned int id = glCreateShader(type));
  const char *src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

  if (result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

    char *message = (char *)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));

    std::cout << "Failed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << " shader!" << std::endl;
    std::cout << message << std::endl;

    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string &vertexShader,
                                 const std::string &fragmentShader) {
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

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
  std::string vertexShader = ParseShader("res/shaders/Basic.vertex.shader");
  std::string fragmentShader = ParseShader("res/shaders/Basic.fragment.shader");

  unsigned int shader = CreateShader(vertexShader, fragmentShader);

  GLCall(int location = glGetUniformLocation(shader, "u_Color"));

  /* Unbind buffers */
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));

  float r = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Use the shader and set it's uniform color */
    GLCall(glUseProgram(shader));
    GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

    /* Select the vertex array buffer */
    va.Bind();
    /* Select the index buffer */
    ib.Bind();

    /* Draw call */
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (r > 1.0f || r < 0.0f)
      increment *= -1;
    r += increment;

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  GLCall(glDeleteProgram(shader));

  glfwTerminate();
  return 0;
}
