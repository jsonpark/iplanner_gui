#include <QtCore/QCoreApplication>
#include <QGuiApplication>
#include <QtGui/QScreen>

#include "iplanner/window/renderer.h"

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);

  QSurfaceFormat format;
  format.setSamples(16);

  iplanner::Renderer window;
  window.setFormat(format);
  window.resize(1280, 720);
  window.show();

  window.SetAnimating(true);

  return a.exec();
}
