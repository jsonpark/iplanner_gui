#ifndef IPLANNER_GL_SHADER_H_
#define IPLANNER_GL_SHADER_H_

#include <glad/glad.h>

#include "iplanner/types.h"

namespace iplanner
{
namespace gl
{
class Shader
{
public:
  enum class Type
  {
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER,
    UNDEFINED,
  };

public:
  // Empty shader
  Shader();

  // Shader that loads file
  Shader(const std::string& filename);

  ~Shader();

  // Copy constructors are deleted
  Shader(const Shader& rhs) = delete;

  Shader& operator = (const Shader& rhs) = delete;

  // Move constructors
  Shader(Shader&& rhs) noexcept;

  Shader& operator = (Shader&& rhs) noexcept;

  void Create();

  auto Id() const noexcept
  {
    return id_;
  }

  void Load(const std::string& filename);

private:
  bool generated_ = false;
  Type type_ = Type::UNDEFINED;

  std::string filename_;
  std::string source_;

  GLuint id_ = 0;
};
}
}

#endif // IPLANNER_GL_SHADER_H_
