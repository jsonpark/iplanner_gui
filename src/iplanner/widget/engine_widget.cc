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
}

EngineWidget::~EngineWidget() = default;

void EngineWidget::initializeGL()
{
  if (!gladLoadGL())
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  const char shader_dir[] = "..\\src\\shader";

  program_ = std::make_unique<gl::Program>(shader_dir, "color_3d");

  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClearDepth(1.f);

  vbo_.Update();

  vao_.AttribPointer(0, 3, vbo_, 6, 0);
  vao_.AttribPointer(1, 3, vbo_, 6, 3);
  vao_.Indices(gl::VertexArray::DrawMode::TRIANGLES, 3);
}

void EngineWidget::paintGL()
{
  const qreal retina_scale = devicePixelRatio();
  glViewport(0, 0, width() * retina_scale, height() * retina_scale);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Affine3f matrix;
  matrix.setIdentity();

  program_->Use();
  program_->UniformMatrix4f("projection", camera_->ProjectionMatrix());
  program_->UniformMatrix4f("view", camera_->ViewMatrix());
  program_->UniformMatrix4f("model", matrix.matrix());

  vao_.Draw();

  program_->Done();
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
