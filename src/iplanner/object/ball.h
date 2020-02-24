#ifndef IPLANNER_OBJECT_BALL_H_
#define IPLANNER_OBJECT_BALL_H_

#include <map>

#include "iplanner/gl/vertex_array.h"
#include "iplanner/gl/buffer.h"
#include "iplanner/types.h"

namespace iplanner
{
class Ball
{
private:
  class Coord
  {
  public:
    Coord() = delete;

    Coord(const Vector3d& coord)
      : coord_(coord)
    {
    }

    bool operator < (const Coord& rhs) const
    {
      constexpr double eps = 1e-5;

      for (int i = 0; i < 3; i++)
      {
        double diff = coord_(i) - rhs.coord_(i);
        if (diff < -eps || diff > eps)
          return diff < 0.;
      }

      return false;
    }

    const Vector3d& Value() const
    {
      return coord_;
    }

  private:
    Vector3d coord_{ 0., 0., 0. };
  };

public:
  Ball() = delete;
  explicit Ball(int subdivision_level = 0);
  ~Ball();

  void Draw();

private:
  void Generate();

  // Vertex index find or create
  int VertexIndex(const Vector3d& v);

  void Subdivide(const Vector3d& p0, const Vector3d& p1, const Vector3d& p2, int level = 0);

  int subdivision_level_ = 0;

  std::map<Coord, int> vertex_idx_;
  std::vector<float> vertex_buffer_;
  std::vector<unsigned int> index_buffer_;

  // OpenGL resource
  bool generated_ = false;
  gl::ArrayBuffer<float> vbo_;
  gl::ElementBuffer elements_;
  gl::VertexArray vao_;
};
}

#endif // IPLANNER_OBJECT_BALL_H_
