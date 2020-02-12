#ifndef IPLANNER_WINDOW_QT_WRAPPER_H_
#define IPLANNER_WINDOW_QT_WRAPPER_H_

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>

namespace iplanner
{
class QtWrapper : public QWindow, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  explicit QtWrapper(QWindow* parent = 0);
  virtual ~QtWrapper();

  virtual void Render(QPainter* painter);
  virtual void Render();

  virtual void Initialize();

public slots:
  void RenderLater();
  void RenderNow();

protected:
  bool event(QEvent* event) override;

  void exposeEvent(QExposeEvent* event) override;

  // Wrapping event functions
  virtual bool Event(QEvent* event);
  virtual void ExposeEvent(QExposeEvent* event);

private:
  QOpenGLContext* context_ = nullptr;
  QOpenGLPaintDevice* device_ = nullptr;

  QOpenGLShaderProgram* program_ = nullptr;
  int frame_ = 0;
};
}

#endif // IPLANNER_WINDOW_QT_WRAPPER_H_
