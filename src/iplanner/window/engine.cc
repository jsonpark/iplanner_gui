#include "iplanner/window/engine.h"

#include <iostream>

#include <QMouseEvent>

namespace iplanner
{
Engine::Engine()
  : QMainWindow()
{
  engine_widget_ = std::make_unique<EngineWidget>();
  setCentralWidget(engine_widget_.get());

  timer_ = std::make_unique<QTimer>();
  connect(timer_.get(), SIGNAL(timeout()), this, SLOT(TimerTick()));
  timer_->start(16);
}

Engine::~Engine() = default;

void Engine::TimerTick()
{
  engine_widget_->update();
}
}
