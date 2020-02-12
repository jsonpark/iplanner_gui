#include "iplanner/gl/buffer.h"

#include <iostream>

namespace iplanner
{
namespace gl
{
BufferBase::BufferBase()
{
}

BufferBase::~BufferBase()
{
  Delete();
}

// Copy constructors
void BufferBase::Copy(const BufferBase& rhs)
{
  if (generated_)
    Delete();

  if (rhs.generated_)
  {
    Generate();

    type_ = rhs.type_;
    buffer_size_ = rhs.buffer_size_;
    usage_ = rhs.usage_;

    if (rhs.buffer_size_ > 0)
    {
      // Allocate Gl buffer
      Bind();
      glBufferData(static_cast<GLenum>(type_), buffer_size_, 0, static_cast<GLenum>(usage_));
      Unbind();

      // Copy buffer elements from rhs
      glCopyNamedBufferSubData(rhs.id_, id_, 0, 0, buffer_size_);
    }
  }
}

BufferBase::BufferBase(const BufferBase& rhs)
{
  Copy(rhs);
}

BufferBase& BufferBase::operator = (const BufferBase& rhs)
{
  Copy(rhs);
  return *this;
}

// Move constructors
void BufferBase::Move(BufferBase&& rhs) noexcept
{
  if (generated_)
    Delete();

  generated_ = rhs.generated_;
  id_ = rhs.id_;
  buffer_size_ = rhs.buffer_size_;
  type_ = rhs.type_;
  usage_ = rhs.usage_;

  rhs.generated_ = false;
  rhs.id_ = 0;
  rhs.buffer_size_ = 0;
  rhs.type_ = BufferType::UNKNOWN;
  rhs.usage_ = BufferUsage::UNKNOWN;
}

BufferBase::BufferBase(BufferBase&& rhs) noexcept
{
  Move(std::move(rhs));
}

BufferBase& BufferBase::operator = (BufferBase&& rhs) noexcept
{
  Move(std::move(rhs));
  return *this;
}

void BufferBase::Generate()
{
  if (!generated_)
  {
    glGenBuffers(1, &id_);
    generated_ = true;
    buffer_size_ = 0;
  }
}

void BufferBase::Delete()
{
  if (generated_)
  {
    glDeleteBuffers(1, &id_);
    generated_ = false;
    buffer_size_ = 0;
  }
}

void BufferBase::Bind()
{
  if (!generated_)
    Generate();

  glBindBuffer(static_cast<GLenum>(type_), id_);
}

void BufferBase::Unbind()
{
  glBindBuffer(static_cast<GLenum>(type_), 0);
}

void BufferBase::Update(BufferType type, int byte_size, void* data)
{
  Update(type, byte_size, data, usage_);
}

void BufferBase::Update(BufferType type, int byte_size, void* data, BufferUsage usage)
{
  if (!generated_)
    Generate();

  // Reallocate if buffer target or usage is different from pre-existing ones
  // or if buffer size is smaller than the new data
  if (type_ != type || usage_ != usage || buffer_size_ < byte_size)
  {
    buffer_size_ = byte_size;
    type_ = type;
    usage_ = usage;

    Bind();
    glBufferData(static_cast<GLenum>(type), byte_size, data, static_cast<GLenum>(usage));
    Unbind();
  }
  else
  {
    Bind();
    glBufferSubData(static_cast<GLenum>(type), 0, byte_size, data);
    Unbind();
  }
}
}
}
