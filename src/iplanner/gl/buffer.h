#ifndef IPLANNER_GL_BUFFER_H_
#define IPLANNER_GL_BUFFER_H_

#include <vector>
#include <initializer_list>

#include <glad/glad.h>

#include "iplanner/types.h"

namespace iplanner
{
namespace gl
{
enum class BufferType
{
  UNKNOWN = -1,
  ARRAY_BUFFER = GL_ARRAY_BUFFER,
  ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
};

enum class BufferUsage
{
  UNKNOWN = -1,
  STATIC_DRAW = GL_STATIC_DRAW,
  DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
  STREAM_DRAW = GL_STREAM_DRAW,
};

class BufferBase
{
public:
  BufferBase();

  virtual ~BufferBase();

  // Copy constructors
  void Copy(const BufferBase& rhs);
  BufferBase(const BufferBase& rhs);
  BufferBase& operator = (const BufferBase& rhs);

  // Move constructors
  void Move(BufferBase&& rhs) noexcept;
  BufferBase(BufferBase&& rhs) noexcept;
  BufferBase& operator = (BufferBase&& rhs) noexcept;

  void Bind();
  void Unbind();

  virtual void Update();

  void Update(BufferType type, int byte_size, void* data);
  void Update(BufferType type, int byte_size, void* data, BufferUsage usage);

protected:
  void Generate();
  void Delete();

  bool generated_ = false;

  GLuint id_ = 0;
  size_t buffer_size_ = 0;
  BufferType type_ = BufferType::UNKNOWN;
  BufferUsage usage_ = BufferUsage::UNKNOWN;
};

template <typename T, BufferType type, BufferUsage usage>
class Buffer : public BufferBase
{
public:
  using data_type = T;

private:
  class Ref
  {
  public:
    Ref() = delete;

    Ref(Buffer& buffer, int idx)
      : buffer_(buffer), idx_(idx)
    {
    }

    ~Ref() = default;

    Ref& operator = (T value)
    {
      buffer_.Modify(idx_, value);
      return *this;
    }

    template <int rows>
    Ref& operator = (const Vector<T, rows>& v)
    {
      for (int i = 0; i < rows; i++)
        buffer_.Modify(idx_ + i, v(i));
      return *this;
    }

    Ref& operator = (const VectorX<T>& v)
    {
      for (int i = 0; i < v.rows(); i++)
        buffer_.Modify(idx_ + i, v(i));
      return *this;
    }

    operator T ()
    {
      return buffer_.data_[idx_];
    }

  private:
    Buffer& buffer_;
    int idx_;
  };

public:
  Buffer()
    : BufferBase()
  {
  }

  Buffer(int size)
    : Buffer(size, static_cast<T>(0))
  {
  }

  Buffer(int size, T value)
    : BufferBase(),
    data_(size, value),
    needs_update_(true)
  {
  }

  Buffer(std::initializer_list<T> values)
    : BufferBase(),
    data_(values),
    needs_update_(true)
  {
  }

  // Copy constructors
  void Copy(const Buffer& rhs)
  {
    BufferBase::Copy(rhs);
    data_ = rhs.data_;
  }

  Buffer(const Buffer& rhs)
  {
    Copy(rhs);
  }

  Buffer& operator = (const Buffer& rhs)
  {
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(Buffer&& rhs) noexcept
  {
    BufferBase::Move(std::move(rhs));
    data_ = std::move(rhs.data_);
  }

  Buffer(Buffer&& rhs) noexcept
  {
    Move(std::move(rhs));
  }

  Buffer& operator = (Buffer&& rhs) noexcept
  {
    Move(std::move(rhs));
    return *this;
  }

  Ref operator [] (int idx)
  {
    return Ref(*this, idx);
  }

  void Resize(int n)
  {
    Resize(n, static_cast<T>(0));
  }

  void Resize(int n, T value)
  {
    if (n != data_.size())
    {
      data_.resize(n, value);
      needs_update_ = true;
    }
  }

  void Modify(int idx, T value)
  {
    if (data_[idx] != value)
    {
      data_[idx] = value;
      needs_update_ = true;
    }
  }

  void Update() override
  {
    if (needs_update_)
    {
      BufferBase::Update(type, data_.size() * sizeof(T), reinterpret_cast<void*>(data_.data()), usage);
      needs_update_ = false;
    }
  }

  auto Size() const
  {
    return data_.size();
  }

private:
  bool needs_update_ = false;
  std::vector<T> data_;
};

template <typename T, BufferUsage usage = BufferUsage::STATIC_DRAW>
using ArrayBuffer = Buffer<T, BufferType::ARRAY_BUFFER, usage>;

using ElementBuffer = Buffer<unsigned int, BufferType::ELEMENT_ARRAY_BUFFER, BufferUsage::STATIC_DRAW>;
}
}

#endif // IPLANNER_GL_BUFFER_H_
