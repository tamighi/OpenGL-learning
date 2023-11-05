#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    std::cout << "Failed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << " shader!" << std::endl;
    std::cout << message << std::endl;

    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string &vertexShader,
                                 const std::string &fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

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

  float positions[6] = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};

  /* Attribute data to the buffer */
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  /* Describe the layout of the attributes in the vertex and enables it*/
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  /* Create shaders */

  std::string vertexShader = ParseShader("res/shaders/Basic.vertex.shader");
  std::string fragmentShader = ParseShader("res/shaders/Basic.fragment.shader");

  unsigned int shader = CreateShader(vertexShader, fragmentShader);
  glUseProgram(shader);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwTerminate();
  return 0;
}
