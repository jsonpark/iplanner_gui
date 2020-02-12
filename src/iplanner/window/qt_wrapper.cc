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

QtWrapper::~QtWrapper()
{
  delete device_;

  if (context_)
    delete context_;
}

void QtWrapper::Render(QPainter* painter)
{
  Q_UNUSED(painter);
}

void QtWrapper::Render()
{
  if (!device_)
    device_ = new QOpenGLPaintDevice;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  device_->setSize(size() * devicePixelRatio());
  device_->setDevicePixelRatio(devicePixelRatio());

  QPainter painter(device_);
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
    context_ = new QOpenGLContext(this);
    context_->setFormat(requestedFormat());
    context_->create();

    needs_initialize = true;
  }

  context_->makeCurrent(this);

  if (needs_initialize)
  {
    initializeOpenGLFunctions();
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

bool QtWrapper::Event(QEvent* e)
{
  return event(e);
}

void QtWrapper::ExposeEvent(QExposeEvent* e)
{
  exposeEvent(e);
}
}
