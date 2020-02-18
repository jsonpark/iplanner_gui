#include "iplanner/gl/vertex_array.h"

#include <utility>

#include <glad/glad.h>

namespace iplanner
{
namespace gl
{
VertexArray::VertexArray() = default;

VertexArray::~VertexArray()
{
  if (generated_)
    Delete();
}

// Copy constructors
void VertexArray::Copy(const VertexArray& rhs)
{
  if (generated_)
    Delete();

  if (rhs.generated_)
    Generate();

  has_element_buffer_ = rhs.has_element_buffer_;
  needs_element_buffer_update_ = rhs.needs_element_buffer_update_;
  num_elements_ = rhs.num_elements_;
  element_buffer_ = rhs.element_buffer_;
  attribs_ = rhs.attribs_;
  needs_attribs_update_ = rhs.needs_attribs_update_;
  mode_ = rhs.mode_;
}

VertexArray::VertexArray(const VertexArray& rhs)
{
  Copy(rhs);
}

VertexArray& VertexArray::operator = (const VertexArray& rhs)
{
  Copy(rhs);
  return *this;
}

// Move constructors
void VertexArray::Move(VertexArray&& rhs) noexcept
{
  if (generated_)
    Delete();


  generated_ = rhs.generated_;
  id_ = rhs.id_;

  has_element_buffer_ = rhs.has_element_buffer_;
  needs_element_buffer_update_ = rhs.needs_element_buffer_update_;
  num_elements_ = rhs.num_elements_;
  element_buffer_ = rhs.element_buffer_;

  attribs_ = std::move(rhs.attribs_);
  needs_attribs_update_ = rhs.needs_attribs_update_;

  mode_ = rhs.mode_;


  rhs.generated_ = false;
  rhs.id_ = 0;
  rhs.has_element_buffer_ = false;
  rhs.needs_element_buffer_update_ = false;
  rhs.num_elements_ = 0;
  rhs.element_buffer_ = nullptr;
  rhs.needs_attribs_update_ = false;
  rhs.mode_ = DrawMode::POINTS;
}

VertexArray::VertexArray(VertexArray&& rhs) noexcept
{
  Move(std::move(rhs));
}

VertexArray& VertexArray::operator = (VertexArray&& rhs) noexcept
{
  Move(std::move(rhs));
  return *this;
}

void VertexArray::Bind()
{
  if (!generated_)
    Generate();

  glBindVertexArray(id_);
}

void VertexArray::Unbind()
{
  glBindVertexArray(0);
}

void VertexArray::Generate()
{
  if (!generated_)
  {
    glGenVertexArrays(1, &id_);
    generated_ = true;
  }
}

void VertexArray::Delete()
{
  if (generated_)
  {
    glDeleteBuffers(1, &id_);
    generated_ = false;
  }
}

void VertexArray::Indices(DrawMode mode, int count)
{
  mode_ = mode;
  has_element_buffer_ = false;
  num_elements_ = count;
}

void VertexArray::Indices(DrawMode mode, ElementBuffer& buffer, int count)
{
  if (count == -1)
    count = buffer.Size();

  buffer.Update();

  mode_ = mode;
  has_element_buffer_ = true;
  element_buffer_ = &buffer;
  num_elements_ = count;
  needs_element_buffer_update_ = true;
}

void VertexArray::UpdateAttribPointer()
{
  if (needs_attribs_update_)
  {
    Bind();

    for (const auto& attrib : attribs_)
    {
      attrib.buffer.get().Bind();
      glVertexAttribPointer(attrib.index, attrib.size, GL_FLOAT, GL_FALSE, attrib.stride, (void*)(static_cast<size_t>(attrib.offset)));
      glEnableVertexAttribArray(attrib.index);
    }

    Unbind();

    needs_attribs_update_ = false;
  }
}

void VertexArray::UpdateElementBuffer()
{
  if (has_element_buffer_ && needs_element_buffer_update_)
  {
    element_buffer_->Update();

    Bind();
    element_buffer_->Bind();
    Unbind();
    element_buffer_->Unbind();

    needs_element_buffer_update_ = false;
  }
}

void VertexArray::Draw()
{
  UpdateAttribPointer();

  if (has_element_buffer_)
    UpdateElementBuffer();

  Bind();

  if (has_element_buffer_)
  {
    glDrawElements(static_cast<GLenum>(mode_), num_elements_, GL_UNSIGNED_INT, 0);
  }

  else
  {
    glDrawArrays(static_cast<GLenum>(mode_), 0, num_elements_);
  }

  Unbind();
}
}
}
