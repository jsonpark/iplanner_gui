#ifndef IPLANNER_WINDOW_ENGINE_H_
#define IPLANNER_WINDOW_ENGINE_H_

#include <QMainWindow>
#include <QTimer>

#include "iplanner/widget/engine_widget.h"

namespace iplanner
{
class Engine : public QMainWindow
{
  Q_OBJECT

public:
  Engine();
  ~Engine();

private slots:
  void TimerTick();

private:
  std::unique_ptr<EngineWidget> engine_widget_;
  std::unique_ptr<QTimer> timer_;
};
}

#endif // IPLANNER_WINDOW_ENGINE_H_
