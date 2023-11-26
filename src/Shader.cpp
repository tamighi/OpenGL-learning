#include "Shader.h"
#include "Renderer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

Shader::Shader(const std::string &vertexSourceFilename,
               const std::string &fragmentSourceFilename)
    : m_RendererID(0) {

  std::string vertexShader = ParseShader(vertexSourceFilename);
  std::string fragmentShader = ParseShader(fragmentSourceFilename);

  m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader() { GLCall(glDeleteProgram(m_RendererID)); }

void Shader::Bind() const { GLCall(glUseProgram(m_RendererID)); }
void Shader::Unbind() const { GLCall(glUseProgram(0)); }

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2,
                          float v3) {

  int location = GetUniformLocation(name);
  GLCall(glUniform4f(location, v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string &name) {
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
    return m_UniformLocationCache[name];
  }

  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1) {
    std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
  }

  m_UniformLocationCache[name] = location;

  return location;
}

std::string Shader::ParseShader(const std::string &filepath) {
  std::ifstream stream(filepath);
  std::stringstream ss;
  std::string line;

  while (std::getline(stream, line)) {
    ss << line << '\n';
  }

  return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type,
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

unsigned int Shader::CreateShader(const std::string &vertexShader,
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
