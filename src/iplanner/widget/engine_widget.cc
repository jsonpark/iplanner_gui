#include "iplanner/widget/engine_widget.h"

#include <iostream>

#include <QMouseEvent>

#include "iplanner/types.h"

namespace iplanner
{
EngineWidget::EngineWidget()
  : QOpenGLWidget()
{
  camera_ = std::make_shared<Camera>();
  camera_->SetEye(0.f, -1.f, 1.f);

  Light light;
  light.type = Light::Type::Directional;
  light.position = Vector3f(-1.f, -1.f, 1.f);
  light.ambient = Vector3f(0.05f, 0.05f, 0.05f);
  light.diffuse = Vector3f(0.3f, 0.3f, 0.3f);
  light.specular = Vector3f(1.f, 1.f, 1.f);
  lights_.push_back(light);

  light.position = Vector3f(1.f, -1.f, 1.f);
  light.ambient = Vector3f(0.05f, 0.05f, 0.05f);
  light.diffuse = Vector3f(0.3f, 0.3f, 0.3f);
  light.specular = Vector3f(1.f, 1.f, 1.f);
  lights_.push_back(light);

  light.position = Vector3f(-1.f, 1.f, 1.f);
  light.ambient = Vector3f(0.05f, 0.05f, 0.05f);
  light.diffuse = Vector3f(0.3f, 0.3f, 0.3f);
  light.specular = Vector3f(1.f, 1.f, 1.f);
  lights_.push_back(light);

  light.position = Vector3f(1.f, 1.f, 1.f);
  light.ambient = Vector3f(0.05f, 0.05f, 0.05f);
  light.diffuse = Vector3f(0.3f, 0.3f, 0.3f);
  light.specular = Vector3f(1.f, 1.f, 1.f);
  lights_.push_back(light);

  material_.ambient = Vector3f(0.5f, 0.5f, 0.5f);
  material_.diffuse = Vector3f(0.5f, 0.5f, 0.5f);
  material_.specular = Vector3f(1.f, 1.f, 1.f);
  material_.shininess = 0.8f;
}

EngineWidget::~EngineWidget() = default;

void EngineWidget::initializeGL()
{
  if (!gladLoadGL())
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glClearColor(1.f, 1.f, 1.f, 1.f);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Ground depth
  ground_depth_vao_.AttribPointer(0, 3, ground_depth_vbo_);
  ground_depth_vao_.Indices(gl::VertexArray::DrawMode::TRIANGLE_STRIP, ground_depth_elements_);

  // Lattice
  lattice_vbo_.Resize(((2 * lattice_num_ + 1) * 2 + 2) * 12);
  for (int i = -lattice_num_; i <= lattice_num_; i++)
  {
    Vector3f color(0.5f, 0.5f, 0.5f);
    if (i == 0)
      color = Vector3f(1.f, 0.f, 0.f);

    lattice_vbo_[12 * (lattice_num_ + i)] = Vector3f(-lattice_num_ * lattice_size_, lattice_size_ * i, 0.f);
    lattice_vbo_[12 * (lattice_num_ + i) + 3] = color;
    lattice_vbo_[12 * (lattice_num_ + i) + 6] = Vector3f(lattice_num_ * lattice_size_, lattice_size_ * i, 0.f);
    lattice_vbo_[12 * (lattice_num_ + i) + 9] = color;
  }
  int base = 12 * (lattice_num_ * 2 + 1);
  for (int i = -lattice_num_; i <= lattice_num_; i++)
  {
    Vector3f color(0.5f, 0.5f, 0.5f);
    if (i == 0)
      color = Vector3f(0.f, 1.f, 0.f);

    lattice_vbo_[base + 12 * (lattice_num_ + i)] = Vector3f(lattice_size_ * i, -lattice_num_ * lattice_size_, 0.f);
    lattice_vbo_[base + 12 * (lattice_num_ + i) + 3] = color;
    lattice_vbo_[base + 12 * (lattice_num_ + i) + 6] = Vector3f(lattice_size_ * i, lattice_num_ * lattice_size_, 0.f);
    lattice_vbo_[base + 12 * (lattice_num_ + i) + 9] = color;
  }

  base = 12 * (lattice_num_ * 2 + 1) * 2;
  lattice_vbo_[base] = Vector3f(0.f, 0.f, 0.f);
  lattice_vbo_[base + 3] = Vector3f(0.f, 0.f, 1.f);
  lattice_vbo_[base + 6] = Vector3f(0.f, 0.f, lattice_num_ * lattice_size_);
  lattice_vbo_[base + 9] = Vector3f(0.f, 0.f, 1.f);

  lattice_vao_.AttribPointer(0, 3, lattice_vbo_, 6, 0);
  lattice_vao_.AttribPointer(1, 3, lattice_vbo_, 6, 3);
  lattice_vao_.Indices(gl::VertexArray::DrawMode::LINES, (lattice_num_ * 2 + 1) * 2 * 2 + 2);

  // Mesh
  mesh_texture_.LoadAsync(mesh_.GetTextureFilename());

  mesh_vbo_positions_ = mesh_.GetPositions();
  mesh_vbo_normals_ = mesh_.GetNormals();
  mesh_vbo_tex_coords_ = mesh_.GetTexCoords();
  mesh_elements_ = mesh_.GetIndices();

  mesh_texture_vao_.AttribPointer(0, 3, mesh_vbo_positions_);
  mesh_texture_vao_.AttribPointer(1, 2, mesh_vbo_tex_coords_);
  mesh_texture_vao_.Indices(gl::VertexArray::DrawMode::TRIANGLES, mesh_elements_);

  mesh_texture_light_vao_.AttribPointer(0, 3, mesh_vbo_positions_);
  mesh_texture_light_vao_.AttribPointer(1, 3, mesh_vbo_normals_);
  mesh_texture_light_vao_.AttribPointer(2, 2, mesh_vbo_tex_coords_);
  mesh_texture_light_vao_.Indices(gl::VertexArray::DrawMode::TRIANGLES, mesh_elements_);

  // Ball
}

void EngineWidget::paintGL()
{
  const qreal retina_scale = devicePixelRatio();
  glViewport(0, 0, width() * retina_scale, height() * retina_scale);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Affine3f matrix;
  matrix.setIdentity();

  ground_depth_program_.Use();
  ground_depth_program_.UniformMatrix4f("projection", camera_->ProjectionMatrix());
  ground_depth_program_.UniformMatrix4f("view", camera_->ViewMatrix());
  ground_depth_program_.UniformMatrix4f("model", matrix.matrix());

  ground_depth_vao_.Draw();

  color_3d_program_.Use();
  color_3d_program_.UniformMatrix4f("projection", camera_->ProjectionMatrix());
  color_3d_program_.UniformMatrix4f("view", camera_->ViewMatrix());
  color_3d_program_.UniformMatrix4f("model", matrix.matrix());

  glDisable(GL_DEPTH_TEST);
  lattice_vao_.Draw();
  glEnable(GL_DEPTH_TEST);

  if (mesh_texture_.IsReady())
  {
    mesh_texture_.Get();
    mesh_texture_object_.Update(mesh_texture_);
  }

  if (mesh_texture_.IsLoaded())
    mesh_texture_object_.Bind();

  // Mesh color
  if (draw_color_)
  {
    mesh_texture_program_.Use();
    mesh_texture_program_.UniformMatrix4f("projection", camera_->ProjectionMatrix());
    mesh_texture_program_.UniformMatrix4f("view", camera_->ViewMatrix());
    mesh_texture_program_.UniformMatrix4f("model", matrix.matrix());

    mesh_texture_vao_.Draw();
  }

  // Mesh light
  if (draw_light_)
  {
    mesh_texture_light_program_.Use();
    mesh_texture_light_program_.UniformMatrix4f("projection", camera_->ProjectionMatrix());
    mesh_texture_light_program_.UniformMatrix4f("view", camera_->ViewMatrix());
    mesh_texture_light_program_.UniformMatrix4f("model", matrix.matrix());
    mesh_texture_light_program_.UniformMatrix3f("model_inv_tp", matrix.matrix().block(0, 0, 3, 3).inverse().transpose());
    mesh_texture_light_program_.Uniform3f("eye_position", camera_->GetEye());

    for (int i = 0; i < 8 && i < lights_.size(); i++)
    {
      const auto& light = lights_[i];

      std::string prefix = "lights[" + std::to_string(i) + "]";
      mesh_texture_light_program_.Uniform1i(prefix + ".use", 1);

      switch (light.type)
      {
      case Light::Type::Directional:
        mesh_texture_light_program_.Uniform1i(prefix + ".type", 0);
        break;

      case Light::Type::Point:
        mesh_texture_light_program_.Uniform1i(prefix + ".type", 1);
        break;
      }

      mesh_texture_light_program_.Uniform3f(prefix + ".position", light.position);
      mesh_texture_light_program_.Uniform3f(prefix + ".ambient", light.ambient);
      mesh_texture_light_program_.Uniform3f(prefix + ".diffuse", light.diffuse);
      mesh_texture_light_program_.Uniform3f(prefix + ".specular", light.specular);
      mesh_texture_light_program_.Uniform3f(prefix + ".attenuation", light.attenuation);
    }
    for (int i = lights_.size(); i < 8; i++)
    {
      std::string prefix = "lights[" + std::to_string(i) + "]";
      mesh_texture_light_program_.Uniform1i(prefix + ".use", 0);
    }

    mesh_texture_light_program_.Uniform3f("material.ambient", material_.ambient);
    mesh_texture_light_program_.Uniform3f("material.diffuse", material_.diffuse);
    mesh_texture_light_program_.Uniform3f("material.specular", material_.specular);
    mesh_texture_light_program_.Uniform1f("material.shininess", material_.shininess);

    mesh_texture_light_program_.Uniform1i("diffuse_texture", 0);

    if (mesh_texture_.IsLoaded())
      mesh_texture_light_program_.Uniform1i("has_diffuse_texture", 1);
    else
      mesh_texture_light_program_.Uniform1i("has_diffuse_texture", 0);

    mesh_texture_light_program_.Uniform1f("alpha", 1.f);

    mesh_texture_light_vao_.Draw();

    mesh_texture_light_program_.Uniform1i("has_diffuse_texture", 0);
    matrix.translate(Vector3f(1.f, 1.f, 1.f));
    matrix.scale(0.1);
    mesh_texture_light_program_.UniformMatrix4f("model", matrix.matrix());
    mesh_texture_light_program_.UniformMatrix3f("model_inv_tp", matrix.matrix().block(0, 0, 3, 3).inverse().transpose());
    mesh_texture_light_program_.Uniform1f("alpha", 1.f);
    ball_.Draw();
  }
}

void EngineWidget::resizeGL(int width, int height)
{
  camera_->SetWidthHeight(width, height);
}

void EngineWidget::mousePressEvent(QMouseEvent* e)
{
  mouse_last_pos_ = Vector2i(e->x(), e->y());
}

void EngineWidget::mouseMoveEvent(QMouseEvent* e)
{
  auto buttons = e->buttons();
  bool left = (buttons & Qt::MouseButton::LeftButton) != 0;
  bool right = (buttons & Qt::MouseButton::RightButton) != 0;

  float dx = e->x() - mouse_last_pos_(0);
  float dy = e->y() - mouse_last_pos_(1);

  if (left && !right)
  {
    camera_->Rotate(dx, dy);
  }

  else if (!left && right)
  {
    camera_->Translate(dx, dy);
  }

  else if (left && right)
  {
    camera_->Zoom(dy);
  }

  mouse_last_pos_ = Vector2i(e->x(), e->y());
}

void EngineWidget::RenderMouseInteractionFramebuffer()
{
}
}
