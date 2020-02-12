#include "iplanner/window/renderer.h"

#include <iostream>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QScreen>

namespace iplanner
{
Renderer::Renderer(QWindow* parent)
  : QtWrapper(parent)
{
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
  const char shader_dir[] = "..\\src\\shader\\";

  program_ = std::make_unique<QOpenGLShaderProgram>();
  program_->addShaderFromSourceFile(QOpenGLShader::Vertex, QString(shader_dir) + "test.vert");
  program_->addShaderFromSourceFile(QOpenGLShader::Fragment, QString(shader_dir) + "test.frag");
  program_->link();

  glClearColor(1.f, 1.f, 1.f, 1.f);

  vbo_.Update();

  vao_.AttribPointer(0, 2, vbo_, 5, 0);
  vao_.AttribPointer(1, 3, vbo_, 5, 2);
  vao_.Indices(gl::VertexArray::DrawMode::TRIANGLES, 3);
}

void Renderer::Render()
{
  const qreal retina_scale = devicePixelRatio();
  glViewport(0, 0, width() * retina_scale, height() * retina_scale);

  glClear(GL_COLOR_BUFFER_BIT);

  program_->bind();

  QMatrix4x4 matrix;
  matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  matrix.translate(0, 0, -2);
  matrix.rotate(100.0f * static_cast<double>(frame_) / screen()->refreshRate(), 0, 1, 0);

  program_->setUniformValue(program_->uniformLocation("matrix"), matrix);

  vao_.Draw();

  program_->release();

  if (animating_)
  {
    requestUpdate();
  }
}

void Renderer::SetAnimating(bool animating)
{
  animating_ = animating;

  if (animating_)
    RenderLater();
}
}
