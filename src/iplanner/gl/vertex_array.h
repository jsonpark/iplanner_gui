#ifndef IPLANNER_GL_VERTEX_ARRAY_H_
#define IPLANNER_GL_VERTEX_ARRAY_H_

#include "iplanner/gl/buffer.h"

namespace iplanner
{
namespace gl
{
class VertexArray
{
public:
  enum class DrawMode
  {
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
  };

public:
  VertexArray();

  ~VertexArray();

  // Copy constructors
  void Copy(const VertexArray& rhs);
  VertexArray(const VertexArray& rhs);
  VertexArray& operator = (const VertexArray& rhs);

  // Move constructors
  void Move(VertexArray&& rhs) noexcept;
  VertexArray(VertexArray&& rhs) noexcept;
  VertexArray& operator = (VertexArray&& rhs) noexcept;

  void Bind();
  void Unbind();

  template <typename T, BufferType type, BufferUsage usage>
  void AttribPointer(int index, int size, Buffer<T, type, usage>& buffer, int stride = 0, int offset = 0)
  {
    attribs_.emplace_back(buffer, index, size, stride * static_cast<int>(sizeof(T)), offset * static_cast<int>(sizeof(T)));
    needs_attribs_update_ = true;
  }

  void Indices(DrawMode mode, int count);
  void Indices(DrawMode mode, ElementBuffer& buffer, int count = -1);

  void Draw();

private:
  void Generate();
  void Delete();

  void UpdateAttribPointer();
  void UpdateElementBuffer();

  bool generated_ = false;
  GLuint id_ = 0;

  bool has_element_buffer_ = false;
  bool needs_element_buffer_update_ = false;
  int num_elements_ = 0;
  ElementBuffer* element_buffer_ = nullptr;

  struct Attrib
  {
    std::reference_wrapper<BufferBase> buffer;
    int index;
    int size;

    // stride and offset in bytes
    int stride;
    int offset;

    Attrib() = delete;

    Attrib(BufferBase& buffer, int index, int size, int stride, int offset)
      : buffer(buffer), index(index), size(size), stride(stride), offset(offset)
    {
    }
  };

  std::vector<Attrib> attribs_;
  bool needs_attribs_update_ = false;

  DrawMode mode_ = DrawMode::POINTS;
};
}
}

#endif // IPLANNER_GL_VERTEX_ARRAY_H_
