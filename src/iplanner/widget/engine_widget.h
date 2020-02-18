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
    0.f, 0.f, -1.f, 1.0f, 0.0f, 0.0f,
    1.f, 0.f, 1.f, 0.0f, 1.0f, 0.0f,
    0.f, 1.f, 1.f, 0.0f, 0.0f, 1.0f
  };

  double lattice_size_ = 1.;
  int lattice_num_ = 10;
  gl::ArrayBuffer<float> lattice_vbo_;
  gl::VertexArray lattice_vao_;

  gl::ArrayBuffer<float> ground_depth_vbo_
  {
    -10.f, -10.f, 0.f,
     10.f, -10.f, 0.f,
    -10.f,  10.f, 0.f,
     10.f,  10.f, 0.f,
  };
  gl::ElementBuffer ground_depth_elements_{ 0, 1, 2, 3 };
  gl::VertexArray ground_depth_vao_;

  gl::Program color_3d_program_{ "..\\src\\shader", "color_3d"};
  gl::Program ground_depth_program_{ "..\\src\\shader", "ground_depth" };

  std::shared_ptr<Camera> camera_;

  // Mouse drag
  Vector2i mouse_last_pos_;
};
}

#endif // IPLANNER_WIDGET_ENGINE_WIDGET_H_
