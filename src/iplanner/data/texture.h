#ifndef IPLANNER_DATA_TEXTURE_H_
#define IPLANNER_DATA_TEXTURE_H_

#include <string>

#include "iplanner/types.h"
#include "iplanner/utils/async_loader.h"
#include "iplanner/utils/stbi_load_helper.h"

namespace iplanner
{
class TextureBase
{
public:
  TextureBase() = default;
  virtual ~TextureBase() = default;

  // Copy constructors
  void Copy(const TextureBase& rhs)
  {
  }

  TextureBase(const TextureBase& rhs)
  {
    Copy(rhs);
  }

  TextureBase& operator = (const TextureBase& rhs)
  {
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(TextureBase&& rhs) noexcept
  {
  }

  TextureBase(TextureBase&& rhs) noexcept
  {
    Move(std::move(rhs));
  }

  TextureBase& operator = (TextureBase&& rhs) noexcept
  {
    Move(std::move(rhs));
    return *this;
  }

private:
};

template <typename T = unsigned char>
class Texture : public TextureBase, public AsyncLoader
{
private:
  class Ref
  {
  public:
    Ref() = delete;

    Ref(Texture& texture, int idx)
      : texture_(texture), idx_(idx)
    {
    }

    ~Ref()
    {
    }

    T& R()
    {
      return texture_.data_[idx_ + 0];
    }

    T& G()
    {
      return texture_.data_[idx_ + 1];
    }

    T& B()
    {
      return texture_.data_[idx_ + 2];
    }

    T& A()
    {
      return texture_.data_[idx_ + 3];
    }

    Ref& operator = (T v)
    {
      texture_.data_[idx_] = v;
      return *this;
    }

    Ref& operator = (const VectorX<T>& v)
    {
      for (int i = 0; i < v.rows(); i++)
        texture_.data_[idx_ + i] = v(i);
      return *this;
    }

    template<int rows>
    Ref& operator = (const Vector<T, rows>& v)
    {
      for (int i = 0; i < v.rows(); i++)
        texture_.data_[idx_ + i] = v(i);
      return *this;
    }

  private:
    Texture& texture_;
    int idx_;
  };

public:
  Texture() = default;

  explicit Texture(const std::string& filename)
  {
    Load(filename);
  }

  Texture(int width, int height, int num_components)
    : width_(width), height_(height), num_components_(num_components),
    data_(width* height* num_components, static_cast<T>(0))
  {
  }

  ~Texture() override = default;

  // Copy constructors do not copy the asynchronously loading texture
  void Copy(const Texture& rhs)
  {
    width_ = rhs.width_;
    height_ = rhs.height_;
    num_components_ = rhs.num_components_;
    data_ = rhs.data_;
  }

  Texture(const Texture& rhs)
    : TextureBase(rhs), AsyncLoader(rhs)
  {
    Copy(rhs);
  }

  Texture& operator = (const Texture& rhs)
  {
    AsyncLoader::Copy(rhs);
    TextureBase::Copy(rhs);
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(Texture&& rhs) noexcept
  {
    width_ = rhs.width_;
    height_ = rhs.height_;
    num_components_ = rhs.num_components_;
    data_ = std::move(rhs.data_);

    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.num_components_ = 0;
  }

  Texture(Texture&& rhs) noexcept
    : TextureBase(std::move(rhs)), AsyncLoader(std::move(rhs))
  {
    Move(std::move(rhs));
  }

  Texture& operator = (Texture&& rhs) noexcept
  {
    TextureBase::Move(std::move(rhs));
    AsyncLoader::Move(std::move(rhs));
    Move(std::move(rhs));
    return *this;
  }

  bool LoadFunc(const std::string& filename) override
  {
    std::cout << "Not implemented" << std::endl;
    return false;
  }

  auto Width() const
  {
    return width_;
  }

  auto Height() const
  {
    return height_;
  }

  auto NumComponents() const
  {
    return num_components_;
  }

  const auto& Data() const
  {
    return data_;
  }

  Ref operator () (int s, int t)
  {
    return Ref(*this, (s + t * width_) * num_components_);
  }

private:
  // left-to-right from (0, 0), bottom-to-top to (width-1, height-1)
  int width_ = 0;
  int height_ = 0;
  int num_components_ = 0;
  std::vector<T> data_;
};

template <>
bool Texture<unsigned char>::LoadFunc(const std::string& filename)
{
  stbi_set_flip_vertically_on_load_helper(true);

  auto data = stbi_load_helper(filename.c_str(), &width_, &height_, &num_components_, 0);
  if (data == NULL)
    return false;

  data_.resize(static_cast<size_t>(width_)* height_* num_components_);
  std::copy(data, data + (static_cast<size_t>(width_)* height_* num_components_), data_.data());

  stbi_image_free_helper(data);

  return true;
}

template <>
bool Texture<float>::LoadFunc(const std::string& filename)
{
  // TODO
  return false;
}



template<typename T = unsigned char>
class Texture1D : public TextureBase
{
private:
  class Ref
  {
  public:
    Ref(Texture1D& texture, int idx)
      : texture_(texture), idx_(idx)
    {
    }

    ~Ref() = default;

    T& R()
    {
      return texture_.data_[idx_ + 0];
    }

    T& G()
    {
      return texture_.data_[idx_ + 1];
    }

    T& B()
    {
      return texture_.data_[idx_ + 2];
    }

    T& A()
    {
      return texture_.data_[idx_ + 3];
    }

    Ref& operator = (const VectorX<T>& v)
    {
      for (int i = 0; i < v.rows(); i++)
        texture_.data_[idx_ + i] = v(i);
      return *this;
    }

    template<int rows>
    Ref& operator = (const Vector<T, rows>& v)
    {
      for (int i = 0; i < v.rows(); i++)
        texture_.data_[idx_ + i] = v(i);
      return *this;
    }

  private:
    Texture1D& texture_;
    int idx_;
  };

public:
  Texture1D() = default;

  Texture1D(int length, int num_components)
    : length_(length), num_components_(num_components), data_(length* num_components, static_cast<T>(0))
  {
  }

  ~Texture1D() override = default;

  // Copy constructors
  void Copy(const Texture1D& rhs)
  {
  }

  Texture1D(const Texture1D& rhs)
    : TextureBase(rhs)
  {
    Copy(rhs);
  }

  Texture1D& operator = (const Texture1D& rhs)
  {
    TextureBase::Copy(rhs);
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(Texture1D&& rhs)
  {
  }

  Texture1D(Texture1D&& rhs)
    : TextureBase(std::move(rhs))
  {
    Move(std::move(rhs));
  }

  Texture1D& operator = (Texture1D&& rhs)
  {
    TextureBase::Move(std::move(rhs));
    Move(std::move(rhs));
    return *this;
  }

  Ref operator () (int s)
  {
    return Ref(*this, s * num_components_);
  }


  int Length() const
  {
    return length_;
  }

  auto NumComponents() const
  {
    return num_components_;
  }

  const auto& Data() const
  {
    return data_;
  }

  void Resize(int n)
  {
    Resize(n, static_cast<T>(0));
  }

  void Resize(int n, T v)
  {
    length_ = n;
    data_.resize(n * num_components_, v);
  }

private:
  int length_ = 1;
  int num_components_ = 0;
  std::vector<T> data_;
};
}

#endif // IPLANNER_DATA_TEXTURE_H_
