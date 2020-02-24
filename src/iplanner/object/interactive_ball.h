#ifndef IPLANNER_OBJECT_INTERACTIVE_BALL_H_
#define IPLANNER_OBJECT_INTERACTIVE_BALL_H_

#include <functional>

#include "iplanner/gl/program.h"
#include "iplanner/object/ball.h"
#include "iplanner/types.h"

namespace iplanner
{
class InteractiveBall
{
public:
  InteractiveBall() = delete;
  InteractiveBall(Ball& ball);
  ~InteractiveBall();

  void Render(gl::Program& program);
  void RenderMouseInteractionFramebuffer(gl::Program& program);

private:
  double radius_ = 0.05;
  Vector3d center_{ 0., 0., 0. };
  Quaterniond q_;
  std::reference_wrapper<Ball> rendering_ball_;
};
}

#endif // IPLANNER_OBJECT_INTERACTIVE_BALL_H_
