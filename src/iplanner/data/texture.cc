#include "iplanner/data/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace iplanner
{
template <>
bool Texture<unsigned char>::LoadFunc(const std::string& filename)
{
  stbi_set_flip_vertically_on_load(true);

  auto data = stbi_load(filename.c_str(), &width_, &height_, &num_components_, 0);
  if (data == NULL)
    return false;

  data_.resize(static_cast<size_t>(width_)* height_* num_components_);
  std::copy(data, data + (static_cast<size_t>(width_)* height_* num_components_), data_.data());

  stbi_image_free(data);

  return true;
}

template <>
bool Texture<float>::LoadFunc(const std::string& filename)
{
  // TODO
  return false;
}
}
