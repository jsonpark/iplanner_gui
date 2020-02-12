#ifndef IPLANNER_WINDOW_RENDERER_H_
#define IPLANNER_WINDOW_RENDERER_H_

#include "iplanner/window/qt_wrapper.h"

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>

namespace iplanner
{
class Renderer : public QtWrapper
{
  Q_OBJECT

public:
  explicit Renderer(QWindow* parent = 0);
  ~Renderer() override;

  void Render(QPainter* painter) override;
  void Render() override;

  void Initialize() override;

  void SetAnimating(bool animating);

private:
  bool animating_ = false;

  GLuint pos_attr_ = 0;
  GLuint col_attr_ = 0;
  GLuint matrix_uniform_ = 0;

  QOpenGLContext* context_ = nullptr;
  QOpenGLPaintDevice* device_ = nullptr;

  QOpenGLShaderProgram* program_ = nullptr;
  int frame_ = 0;
};
}

#endif // IPLANNER_WINDOW_RENDERER_H_
