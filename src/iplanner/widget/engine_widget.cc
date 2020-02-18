#include "iplanner/widget/engine_widget.h"

#include <iostream>

#include <QMouseEvent>

#include "iplanner/types.h"

namespace iplanner
{
EngineWidget::EngineWidget()
  : QOpenGLWidget()
{
  camera_ = std::make_shared<Camera>();
  camera_->SetEye(0.f, -1.f, 1.f);
}

EngineWidget::~EngineWidget() = default;

void EngineWidget::initializeGL()
{
  if (!gladLoadGL())
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glClearColor(1.f, 1.f, 1.f, 1.f);
  glEnable(GL_DEPTH_TEST);

  // Test triangle vao
  vao_.AttribPointer(0, 3, vbo_, 6, 0);
  vao_.AttribPointer(1, 3, vbo_, 6, 3);
  vao_.Indices(gl::VertexArray::DrawMode::TRIANGLES, 3);

  // Ground depth
  ground_depth_vao_.AttribPointer(0, 3, ground_depth_vbo_);
  ground_depth_vao_.Indices(gl::VertexArray::DrawMode::TRIANGLE_STRIP, ground_depth_elements_);

  // Lattice
  lattice_vbo_.Resize(((2 * lattice_num_ + 1) * 2 + 2) * 12);
  for (int i = -lattice_num_; i <= lattice_num_; i++)
  {
    Vector3f color(0.5f, 0.5f, 0.5f);
    if (i == 0)
      color = Vector3f(1.f, 0.f, 0.f);

    lattice_vbo_[12 * (lattice_num_ + i)] = Vector3f(-lattice_num_ * lattice_size_, lattice_size_ * i, 0.f);
    lattice_vbo_[12 * (lattice_num_ + i) + 3] = color;
    lattice_vbo_[12 * (lattice_num_ + i) + 6] = Vector3f(lattice_num_ * lattice_size_, lattice_size_ * i, 0.f);
    lattice_vbo_[12 * (lattice_num_ + i) + 9] = color;
  }
  int base = 12 * (lattice_num_ * 2 + 1);
  for (int i = -lattice_num_; i <= lattice_num_; i++)
  {
    Vector3f color(0.5f, 0.5f, 0.5f);
    if (i == 0)
      color = Vector3f(0.f, 1.f, 0.f);

    lattice_vbo_[base + 12 * (lattice_num_ + i)] = Vector3f(lattice_size_ * i, -lattice_num_ * lattice_size_, 0.f);
    lattice_vbo_[base + 12 * (lattice_num_ + i) + 3] = color;
    lattice_vbo_[base + 12 * (lattice_num_ + i) + 6] = Vector3f(lattice_size_ * i, lattice_num_ * lattice_size_, 0.f);
    lattice_vbo_[base + 12 * (lattice_num_ + i) + 9] = color;
  }

  base = 12 * (lattice_num_ * 2 + 1) * 2;
  lattice_vbo_[base] = Vector3f(0.f, 0.f, 0.f);
  lattice_vbo_[base + 3] = Vector3f(0.f, 0.f, 1.f);
  lattice_vbo_[base + 6] = Vector3f(0.f, 0.f, lattice_num_ * lattice_size_);
  lattice_vbo_[base + 9] = Vector3f(0.f, 0.f, 1.f);

  lattice_vao_.AttribPointer(0, 3, lattice_vbo_, 6, 0);
  lattice_vao_.AttribPointer(1, 3, lattice_vbo_, 6, 3);
  lattice_vao_.Indices(gl::VertexArray::DrawMode::LINES, (lattice_num_ * 2 + 1) * 2 * 2 + 2);
}

void EngineWidget::paintGL()
{
  const qreal retina_scale = devicePixelRatio();
  glViewport(0, 0, width() * retina_scale, height() * retina_scale);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Affine3f matrix;
  matrix.setIdentity();

  ground_depth_program_.Use();
  ground_depth_program_.UniformMatrix4f("projection", camera_->ProjectionMatrix());
  ground_depth_program_.UniformMatrix4f("view", camera_->ViewMatrix());
  ground_depth_program_.UniformMatrix4f("model", matrix.matrix());

  ground_depth_vao_.Draw();

  color_3d_program_.Use();
  color_3d_program_.UniformMatrix4f("projection", camera_->ProjectionMatrix());
  color_3d_program_.UniformMatrix4f("view", camera_->ViewMatrix());
  color_3d_program_.UniformMatrix4f("model", matrix.matrix());

  glDisable(GL_DEPTH_TEST);
  lattice_vao_.Draw();

  glEnable(GL_DEPTH_TEST);
  vao_.Draw();

  color_3d_program_.Done();
}

void EngineWidget::resizeGL(int width, int height)
{
  camera_->SetWidthHeight(width, height);
}

void EngineWidget::mousePressEvent(QMouseEvent* e)
{
  mouse_last_pos_ = Vector2i(e->x(), e->y());
}

void EngineWidget::mouseMoveEvent(QMouseEvent* e)
{
  auto buttons = e->buttons();
  bool left = (buttons & Qt::MouseButton::LeftButton) != 0;
  bool right = (buttons & Qt::MouseButton::RightButton) != 0;

  float dx = e->x() - mouse_last_pos_(0);
  float dy = e->y() - mouse_last_pos_(1);

  if (left && !right)
  {
    camera_->Rotate(dx, dy);
  }

  else if (!left && right)
  {
    camera_->Translate(dx, dy);
  }

  else if (left && right)
  {
    camera_->Zoom(dy);
  }

  mouse_last_pos_ = Vector2i(e->x(), e->y());
}
}
