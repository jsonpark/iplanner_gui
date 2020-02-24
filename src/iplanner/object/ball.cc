#include "iplanner/object/ball.h"

namespace iplanner
{
Ball::Ball(int subdivision_level)
  : subdivision_level_(subdivision_level)
{
  Subdivide(Vector3d(0., 0., 1.), Vector3d(1., 0., 0.), Vector3d(0., 1., 0.));
  Subdivide(Vector3d(0., 0., 1.), Vector3d(0., 1., 0.), Vector3d(-1., 0., 0.));
  Subdivide(Vector3d(0., 0., 1.), Vector3d(-1., 0., 0.), Vector3d(0., -1., 0.));
  Subdivide(Vector3d(0., 0., 1.), Vector3d(0., -1., 0.), Vector3d(1., 0., 0.));

  Subdivide(Vector3d(0., 0., -1.), Vector3d(0., 1., 0.), Vector3d(1., 0., 0.));
  Subdivide(Vector3d(0., 0., -1.), Vector3d(1., 0., 0.), Vector3d(0., -1., 0.));
  Subdivide(Vector3d(0., 0., -1.), Vector3d(0., -1., 0.), Vector3d(-1., 0., 0.));
  Subdivide(Vector3d(0., 0., -1.), Vector3d(-1., 0., 0.), Vector3d(0., 1., 0.));

  vertex_buffer_.resize(vertex_idx_.size() * 3);
  for (auto it : vertex_idx_)
  {
    int idx = it.second;
    const auto& p = it.first.Value();

    vertex_buffer_[idx * 3 + 0] = p(0);
    vertex_buffer_[idx * 3 + 1] = p(1);
    vertex_buffer_[idx * 3 + 2] = p(2);
  }
}

Ball::~Ball() = default;

int Ball::VertexIndex(const Vector3d& v)
{
  auto it = vertex_idx_.find(v);
  if (it == vertex_idx_.cend())
  {
    int idx = vertex_idx_.size();
    vertex_idx_[v] = idx;
    return idx;
  }
  return it->second;
}

void Ball::Subdivide(const Vector3d& p0, const Vector3d& p1, const Vector3d& p2, int level)
{
  if (level == subdivision_level_)
  {
    int i0 = VertexIndex(p0);
    int i1 = VertexIndex(p1);
    int i2 = VertexIndex(p2);

    index_buffer_.push_back(i0);
    index_buffer_.push_back(i1);
    index_buffer_.push_back(i2);

    return;
  }

  Vector3d p01 = (p0 + p1).normalized();
  Vector3d p12 = (p1 + p2).normalized();
  Vector3d p20 = (p2 + p0).normalized();
  Subdivide(p0, p01, p20, level + 1);
  Subdivide(p1, p12, p01, level + 1);
  Subdivide(p2, p20, p12, level + 1);
  Subdivide(p01, p12, p20, level + 1);
}

void Ball::Draw()
{
  if (!generated_)
    Generate();

  vao_.Draw();
}

void Ball::Generate()
{
  if (!generated_)
  {
    generated_ = true;
    vbo_ = vertex_buffer_;
    vao_.AttribPointer(0, 3, vbo_);
    vao_.AttribPointer(1, 3, vbo_);
    // TODO: tex coords

    elements_ = index_buffer_;
    vao_.Indices(gl::VertexArray::DrawMode::TRIANGLES, elements_);
  }
}
}
