#ifndef IPLANNER_GL_GL_TEXTURE_H_
#define IPLANNER_GL_GL_TEXTURE_H_

#include <utility>

#include <glad/glad.h>

#include "iplanner/data/texture.h"

namespace iplanner
{
namespace gl
{
class TextureObject
{
public:
  enum class Wrap
  {
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    REPEAT = GL_REPEAT,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
  };

  enum class MinFilter
  {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  };

  enum class MagFilter
  {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
  };

protected:
  enum class Type
  {
    UNKNOWN = 0,
    TEXTURE_1D = GL_TEXTURE_1D,
    TEXTURE_2D = GL_TEXTURE_2D,
  };

public:
  TextureObject() = delete;

  TextureObject(Type type)
    : type_(type)
  {
  }

  virtual ~TextureObject()
  {
    Delete();
  }

  // Copy constructors
  void Copy(const TextureObject& rhs)
  {
    id_ = rhs.id_;
    need_update_ = rhs.need_update_;
    type_ = rhs.type_;
  }

  TextureObject(const TextureObject& rhs)
  {
    Copy(rhs);
  }

  TextureObject& operator = (const TextureObject& rhs)
  {
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(TextureObject&& rhs) noexcept
  {
    id_ = rhs.id_;
    need_update_ = rhs.need_update_;
    type_ = rhs.type_;

    rhs.id_ = 0;
    rhs.need_update_ = false;
    rhs.type_ = Type::UNKNOWN;
  }


  TextureObject(TextureObject&& rhs) noexcept
  {
    Move(std::move(rhs));
  }

  TextureObject& operator = (TextureObject&& rhs) noexcept
  {
    Move(std::move(rhs));
    return *this;
  }


  void Bind()
  {
    if (!generated_)
      Generate();

    glBindTexture(static_cast<GLenum>(type_), id_);
  }

  void Unbind()
  {
    glBindTexture(static_cast<GLenum>(type_), 0);
  }

  void WrapS(Wrap wrap)
  {
    wrap_s_ = wrap;
    need_filter_update_ = true;
  }

  void WrapT(Wrap wrap)
  {
    wrap_t_ = wrap;
    need_filter_update_ = true;
  }

  void MinFilter(typename MinFilter filter)
  {
    min_filter_ = filter;
    need_filter_update_ = true;
  }

  void MagFilter(typename MagFilter filter)
  {
    mag_filter_ = filter;
    need_filter_update_ = true;
  }

  void UpdateFilters()
  {
    if (need_filter_update_)
    {
      glTexParameteri(static_cast<GLenum>(type_), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s_));
      glTexParameteri(static_cast<GLenum>(type_), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t_));
      glTexParameteri(static_cast<GLenum>(type_), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter_));
      glTexParameteri(static_cast<GLenum>(type_), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter_));

      need_filter_update_ = false;
    }
  }

  void Generate()
  {
    if (!generated_)
    {
      glGenTextures(1, &id_);
      generated_ = true;
    }
  }

  void Delete()
  {
    if (generated_)
    {
      glDeleteTextures(1, &id_);
      generated_ = false;
    }
  }

protected:
  auto GetType() const
  {
    return type_;
  }

private:
  bool generated_ = false;
  GLuint id_ = 0;
  Type type_ = Type::UNKNOWN;

  Wrap wrap_s_ = Wrap::REPEAT;
  Wrap wrap_t_ = Wrap::REPEAT;
  enum class MinFilter min_filter_ = MinFilter::LINEAR;
  enum class MagFilter mag_filter_ = MagFilter::LINEAR;

  bool need_update_ = false;
  bool need_filter_update_ = false;
};


template <typename T>
class TextureObject2D : public TextureObject
{
public:
  TextureObject2D()
    : TextureObject(Type::TEXTURE_2D)
  {
    WrapS(Wrap::REPEAT);
    WrapT(Wrap::REPEAT);
    MinFilter(MinFilter::LINEAR_MIPMAP_LINEAR);
    MagFilter(MagFilter::LINEAR);
  }

  ~TextureObject2D() override = default;

  void Update(const Texture<T>& texture)
  {
    GLenum internal_format = GL_RGBA;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    switch (texture.NumComponents())
    {
    case 1:
      internal_format = GL_RED;
      format = GL_RED;
      break;

    case 2:
      internal_format = GL_RG;
      format = GL_RG;
      break;

    case 3:
      internal_format = GL_RGB;
      format = GL_RGB;
      break;

    case 4:
      internal_format = GL_RGBA;
      format = GL_RGBA;
      break;

    default:
      break;
    }

    Bind();
    glTexImage2D(static_cast<GLenum>(GetType()), 0, internal_format, texture.Width(), texture.Height(), 0, format, type, texture.Data().data());
    glGenerateMipmap(static_cast<GLenum>(GetType()));
    UpdateFilters();
    Unbind();
  }

private:
};


template <typename T>
class TextureObject1D : public TextureObject
{
public:
  TextureObject1D()
    : TextureObject(Type::TEXTURE_1D)
  {
    WrapS(Wrap::REPEAT);
    MinFilter(MinFilter::LINEAR_MIPMAP_LINEAR);
    MagFilter(MagFilter::LINEAR);
  }

  ~TextureObject1D() override = default;

  void Update(const Texture1D<T>& texture)
  {
    GLenum internal_format = GL_RGBA;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    switch (texture.NumComponents())
    {
    case 1:
      internal_format = GL_RED;
      format = GL_RED;
      break;

    case 2:
      internal_format = GL_RG;
      format = GL_RG;
      break;

    case 3:
      internal_format = GL_RGB;
      format = GL_RGB;
      break;

    case 4:
      internal_format = GL_RGBA;
      format = GL_RGBA;
      break;

    default:
      break;
    }

    Bind();
    glTexImage1D(static_cast<GLenum>(GetType()), 0, internal_format, texture.Length(), 0, format, type, texture.Data().data());
    glGenerateMipmap(static_cast<GLenum>(GetType()));
    UpdateFilters();
    Unbind();
  }

private:
};
}
}

#endif // IPLANNER_GL_GL_TEXTURE_H_
