#include "iplanner/object/interactive_ball.h"

namespace iplanner
{
InteractiveBall::InteractiveBall(Ball& ball)
  : rendering_ball_(ball)
{
}

InteractiveBall::~InteractiveBall() = default;

void InteractiveBall::Render(gl::Program& program)
{
  Affine3d transform;
  transform.setIdentity();
  transform.translate(center_);
  transform.scale(radius_);

  Matrix4f model = transform.cast<float>().matrix();

  program.Uniform4f("model", model);
  program.Uniform4f("model_inv_tp", model.block(0, 0, 3, 3).inverse().transpose());

  rendering_ball_.get().Draw();
}

void InteractiveBall::RenderMouseInteractionFramebuffer(gl::Program& program)
{
}
}
