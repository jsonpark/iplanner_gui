#include "iplanner/gl/program.h"

#include <iostream>
#include <filesystem>

namespace iplanner
{
namespace gl
{
Program::Program() = default;

Program::Program(const std::string& path, const std::string& name)
{
  Load(path, name);
}

Program::~Program()
{
  if (created_)
    glDeleteProgram(id_);
}

Program::Program(Program&& rhs) noexcept
{
  created_ = rhs.created_;

  shaders_ = std::move(rhs.shaders_);

  id_ = rhs.id_;

  // Reset basic type variables to default values
  rhs.created_ = false;
  rhs.id_ = 0;
}

Program& Program::operator = (Program&& rhs) noexcept
{
  if (created_)
    glDeleteProgram(id_);

  created_ = rhs.created_;

  shaders_ = std::move(rhs.shaders_);

  id_ = rhs.id_;

  // Reset basic type variables to default values
  rhs.created_ = false;
  rhs.id_ = 0;

  return *this;
}

void Program::Load(const std::string& path, const std::string& name)
{
  auto base_filename = path + "\\" + name;

  if (std::filesystem::exists(base_filename + ".vert"))
    shaders_.emplace_back(base_filename + ".vert");

  if (std::filesystem::exists(base_filename + ".frag"))
    shaders_.emplace_back(base_filename + ".frag");

  if (std::filesystem::exists(base_filename + ".geom"))
    shaders_.emplace_back(base_filename + ".geom");
}

void Program::Create()
{
  if (!created_)
  {
    for (auto& shader : shaders_)
      shader.Create();

    // Generate shader program
    id_ = glCreateProgram();
    created_ = true;

    for (const auto& shader : shaders_)
      glAttachShader(id_, shader.Id());

    glLinkProgram(id_);


    int success;
    glGetProgramiv(id_, GL_LINK_STATUS, &success);

    if (!success)
    {
      char info_log[512];
      glGetProgramInfoLog(id_, 512, NULL, info_log);
      throw std::runtime_error(std::string("Program: Link failed.\n") + info_log);
    }

    // Delete shaders
    shaders_.clear();
  }
}

void Program::Use()
{
  if (!created_)
    Create();

  glUseProgram(id_);
}

void Program::Done()
{
  glUseProgram(0);
}

void Program::Uniform1f(const std::string& name, float v)
{
  if (!created_)
    Create();

  Use();
  glUniform1f(glGetUniformLocation(id_, name.c_str()), v);
}

void Program::Uniform1i(const std::string& name, int i)
{
  if (!created_)
    Create();

  Use();
  glUniform1i(glGetUniformLocation(id_, name.c_str()), i);
}

void Program::Uniform2f(const std::string& name, const Vector2f& v)
{
  if (!created_)
    Create();

  Use();
  glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, v.data());
}

void Program::Uniform2i(const std::string& name, int i0, int i1)
{
  if (!created_)
    Create();

  Use();
  glUniform2i(glGetUniformLocation(id_, name.c_str()), i0, i1);
}

void Program::Uniform3f(const std::string& name, const Vector3f& v)
{
  if (!created_)
    Create();

  Use();
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, v.data());
}

void Program::Uniform4f(const std::string& name, const Vector4f& v)
{
  if (!created_)
    Create();

  Use();
  glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, v.data());
}

void Program::UniformMatrix3f(const std::string& name, const Matrix3f& m)
{
  if (!created_)
    Create();

  Use();
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, m.data());
}

void Program::UniformMatrix4f(const std::string& name, const Matrix4f& m)
{
  if (!created_)
    Create();

  Use();
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, m.data());
}
}
}
