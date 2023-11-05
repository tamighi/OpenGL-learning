#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    __builtin_trap();

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  ASSERT(!GLLogCall(#x, __FILE__, __LINE__));

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

static bool GLLogCall(const char *function, const char *file, int line) {
  bool isError = false;

  while (GLenum error = glGetError()) {
    std::cout << "Error " << error << " in function " << function << " of file "
              << file << " in line " << line << std::endl;
    isError = true;
  }

  return isError;
}

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

  /* Buffer here */
  unsigned int buffer;

  /* Generate a buffer an select it */
  GLCall(glGenBuffers(1, &buffer));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

  /* Attribute data to the buffer */
  GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions,
                      GL_STATIC_DRAW));

  /* Describe the layout of the attributes in the vertex and enables it*/
  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

  /* Buffer here */
  unsigned int ibo;

  /* Generate a buffer an select it */
  GLCall(glGenBuffers(1, &ibo));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

  /* Attribute data to the buffer */
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
                      indices, GL_STATIC_DRAW));

  /* Create shaders */

  std::string vertexShader = ParseShader("res/shaders/Basic.vertex.shader");
  std::string fragmentShader = ParseShader("res/shaders/Basic.fragment.shader");

  unsigned int shader = CreateShader(vertexShader, fragmentShader);
  GLCall(glUseProgram(shader));

  GLCall(int location = glGetUniformLocation(shader, "u_Color"));
  ASSERT(location != -1);

  float r = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (r > 1.0f || r < 0.0f) increment *= -1;
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
