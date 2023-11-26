#pragma once

#include <string>
#include <unordered_map>

class Shader {
public:
  Shader(const std::string &vertexSourceFilename,
         const std::string &fragmentSourceFilename);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void SetUniform4f(const std::string &name, float v0, float v1, float v2,
                    float v3);

private:
  unsigned int m_RendererID;
  std::unordered_map<std::string, int> m_UniformLocationCache;

  int GetUniformLocation(const std::string &name);
  unsigned int CreateShader(const std::string &vertexShader,
                            const std::string &fragmentShader);
  unsigned int CompileShader(unsigned int type, const std::string &source);
  std::string ParseShader(const std::string &filepath);
};
