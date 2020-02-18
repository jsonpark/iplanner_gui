#ifndef IPLANNER_WIDGET_ENGINE_WIDGET_H_
#define IPLANNER_WIDGET_ENGINE_WIDGET_H_

#include <glad/glad.h>

#include <QOpenGLWidget>

#include <QtGui/QOpenGLShaderProgram>

#include "iplanner/gl/buffer.h"
#include "iplanner/gl/vertex_array.h"
#include "iplanner/gl/program.h"
#include "iplanner/data/camera.h"

namespace iplanner
{
class EngineWidget : public QOpenGLWidget
{
  Q_OBJECT

public:
  EngineWidget();
  ~EngineWidget();

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  void mousePressEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;

private:
  int frame_ = 0;

  gl::VertexArray vao_;
  gl::ArrayBuffer<float> vbo_
  {
    0.f, 0.f, 0.f, 1.0f, 0.0f, 0.0f,
    1.f, 0.f, 0.f, 0.0f, 1.0f, 0.0f,
    0.f, 1.f, 0.f, 0.0f, 0.0f, 1.0f
  };

  gl::ArrayBuffer<float> lattice_vbo_;
  gl::VertexArray lattice_vao_;

  std::unique_ptr<gl::Program> program_;

  std::shared_ptr<Camera> camera_;

  // Mouse drag
  Vector2i mouse_last_pos_;
};
}

#endif // IPLANNER_WIDGET_ENGINE_WIDGET_H_
