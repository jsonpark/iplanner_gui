#ifndef IPLANNER_WINDOW_QT_WRAPPER_H_
#define IPLANNER_WINDOW_QT_WRAPPER_H_

#include <glad/glad.h>

#include <QtGui/QWindow>
#include <QtGui/QOpenGLPaintDevice>

namespace iplanner
{
class QtWrapper : public QWindow
{
  Q_OBJECT

public:
  explicit QtWrapper(QWindow* parent = 0);
  virtual ~QtWrapper() override;

  virtual void Render(QPainter* painter);
  virtual void Render();

  virtual void Initialize();

public slots:
  void RenderLater();
  void RenderNow();

protected:
  bool event(QEvent* event) override;

  void exposeEvent(QExposeEvent* event) override;

private:
  std::unique_ptr<QOpenGLContext> context_;
  std::unique_ptr<QOpenGLPaintDevice> device_;
};
}

#endif // IPLANNER_WINDOW_QT_WRAPPER_H_
