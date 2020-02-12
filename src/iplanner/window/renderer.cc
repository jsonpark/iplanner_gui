#include "iplanner/window/renderer.h"

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QScreen>

namespace iplanner
{
namespace
{
static const char* vertex_shader_source =
  "attribute highp vec4 pos_attr;\n"
  "attribute lowp vec4 col_attr;\n"
  "varying lowp vec4 col;\n"
  "uniform highp mat4 matrix;\n"
  "void main() {\n"
  "   col = col_attr;\n"
  "   gl_Position = matrix * pos_attr;\n"
  "}\n";

static const char* fragment_shader_source =
  "varying lowp vec4 col;\n"
  "void main() {\n"
  "   gl_FragColor = col;\n"
  "}\n";
}

Renderer::Renderer(QWindow* parent)
  : QtWrapper(parent)
{
}

Renderer::~Renderer()
{
  delete device_;
}

void Renderer::Render(QPainter* painter)
{
  Q_UNUSED(painter);
}

void Renderer::Render()
{
  const qreal retinaScale = devicePixelRatio();
  glViewport(0, 0, width() * retinaScale, height() * retinaScale);

  glClear(GL_COLOR_BUFFER_BIT);

  program_->bind();

  QMatrix4x4 matrix;
  matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  matrix.translate(0, 0, -2);
  matrix.rotate(100.0f * static_cast<double>(frame_) / screen()->refreshRate(), 0, 1, 0);

  program_->setUniformValue(matrix_uniform_, matrix);

  GLfloat vertices[] = {
      0.0f, 0.707f,
      -0.5f, -0.5f,
      0.5f, -0.5f
  };

  GLfloat colors[] = {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f
  };

  glVertexAttribPointer(pos_attr_, 2, GL_FLOAT, GL_FALSE, 0, vertices);
  glVertexAttribPointer(col_attr_, 3, GL_FLOAT, GL_FALSE, 0, colors);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  program_->release();

  frame_++;

  if (animating_)
    requestUpdate();
}

void Renderer::Initialize()
{
  program_ = new QOpenGLShaderProgram(this);
  program_->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader_source);
  program_->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader_source);
  program_->link();
  pos_attr_ = program_->attributeLocation("pos_attr");
  col_attr_ = program_->attributeLocation("col_attr");
  matrix_uniform_ = program_->uniformLocation("matrix");
}

void Renderer::SetAnimating(bool animating)
{
  animating_ = animating;

  if (animating_)
    RenderLater();
}
}
