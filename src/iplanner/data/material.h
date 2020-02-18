#ifndef IPLANNER_DATA_MATERIAL_H_
#define IPLANNER_DATA_MATERIAL_H_

#include "iplanner/types.h"

namespace iplanner
{
struct Material
{
  Vector3f ambient{ 0.f, 0.f, 0.f };
  Vector3f diffuse{ 0.f, 0.f, 0.f };
  Vector3f specular{ 0.f, 0.f, 0.f };
  float shininess;
};
}

#endif // IPLANNER_DATA_MATERIAL_H_
