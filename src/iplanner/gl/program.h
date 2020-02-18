#ifndef IPLANNER_GL_PROGRAM_H_
#define IPLANNER_GL_PROGRAM_H_

#include "iplanner/gl/shader.h"
#include "iplanner/types.h"

namespace iplanner
{
namespace gl
{
class Program
{
public:
  // Empty shader program
  Program();

  // Load shaders with extensions .vert, .frag, .geom
  Program(const std::string& path, const std::string& name);

  ~Program();

  // Copy constructors are deleted
  Program(const Program& rhs) = delete;

  Program& operator = (const Program& rhs) = delete;

  // Move constructors
  Program(Program&& rhs) noexcept;

  Program& operator = (Program&& rhs) noexcept;


  void Create();

  auto Id() const noexcept
  {
    return id_;
  }

  void Use();
  void Done();

  void Uniform1f(const std::string& name, float v);
  void Uniform1i(const std::string& name, int i);
  void Uniform2f(const std::string& name, const Vector2f& v);
  void Uniform2i(const std::string& name, int i0, int i1);
  void Uniform3f(const std::string& name, const Vector3f& v);
  void Uniform4f(const std::string& name, const Vector4f& v);
  void UniformMatrix3f(const std::string& name, const Matrix3f& m);
  void UniformMatrix4f(const std::string& name, const Matrix4f& m);

private:
  void Load(const std::string& path, const std::string& name);

  bool created_ = false;

  std::vector<Shader> shaders_;

  GLuint id_;
};
}
}

#endif // IPLANNER_GL_PROGRAM_H_
