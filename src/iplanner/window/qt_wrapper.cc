#include "iplanner/window/qt_wrapper.h"

#include <iostream>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QScreen>

namespace iplanner
{
QtWrapper::QtWrapper(QWindow* parent)
  : QWindow(parent)
{
  setSurfaceType(QWindow::OpenGLSurface);
}

QtWrapper::~QtWrapper() = default;

void QtWrapper::Render(QPainter* painter)
{
  Q_UNUSED(painter);
}

void QtWrapper::Render()
{
  if (!device_)
    device_ = std::make_unique<QOpenGLPaintDevice>();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  device_->setSize(size() * devicePixelRatio());
  device_->setDevicePixelRatio(devicePixelRatio());

  QPainter painter(device_.get());
  Render(&painter);
}

void QtWrapper::Initialize()
{
}

void QtWrapper::RenderLater()
{
  requestUpdate();
}

void QtWrapper::RenderNow()
{
  if (!isExposed())
    return;

  bool needs_initialize = false;

  if (context_ == nullptr)
  {
    context_ = std::make_unique<QOpenGLContext>();
    context_->setFormat(requestedFormat());
    context_->create();

    needs_initialize = true;
  }

  context_->makeCurrent(this);

  if (needs_initialize)
  {
    // Using OpenGL functions inherited and loaded by QOpenGLFunctions_4_4_Core
    //initializeOpenGLFunctions();

    // Using glad
    if (!gladLoadGL())
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return;
    }

    Initialize();
  }

  Render();

  context_->swapBuffers(this);
}

bool QtWrapper::event(QEvent* event)
{
  switch (event->type())
  {
  case QEvent::UpdateRequest:
    RenderNow();
    return true;

  default:
    return QWindow::event(event);
  }
}

void QtWrapper::exposeEvent(QExposeEvent* event)
{
  Q_UNUSED(event);

  if (isExposed())
    RenderNow();
}
}
