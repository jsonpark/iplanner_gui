#include <iostream>

//#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QtGui/QScreen>

#include "iplanner/window/engine.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QSurfaceFormat format;
  format.setVersion(4, 4);
  format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);

  iplanner::Engine engine;
  engine.resize(1280, 720);
  engine.show();

  return a.exec();
}
