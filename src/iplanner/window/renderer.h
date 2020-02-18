#ifndef IPLANNER_WINDOW_RENDERER_H_
#define IPLANNER_WINDOW_RENDERER_H_

#include "iplanner/window/qt_wrapper.h"

#include <QtGui/QOpenGLShaderProgram>

#include "iplanner/gl/buffer.h"
#include "iplanner/gl/vertex_array.h"

namespace iplanner
{
class Renderer : public QtWrapper
{
  Q_OBJECT

public:
  explicit Renderer(QWindow* parent = 0);
  ~Renderer() override;

  void Initialize() override;
  void Render() override;

  void SetAnimating(bool animating);

protected:
  void mouseMoveEvent(QMouseEvent* e) override;
  void mousePressEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;
  void keyPressEvent(QKeyEvent* ev) override;
  void keyReleaseEvent(QKeyEvent* ev) override;

private:
  bool animating_ = false;
  int frame_ = 0;

  gl::VertexArray vao_;
  gl::ArrayBuffer<float> vbo_
  {
    0.0f, 0.707f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f
  };

  std::unique_ptr<QOpenGLShaderProgram> program_;
};
}

#endif // IPLANNER_WINDOW_RENDERER_H_
