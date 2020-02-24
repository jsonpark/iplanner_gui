#ifndef IPLANNER_WIDGET_ENGINE_WIDGET_H_
#define IPLANNER_WIDGET_ENGINE_WIDGET_H_

#include <glad/glad.h>

#include <QOpenGLWidget>

#include <QtGui/QOpenGLShaderProgram>

#include "iplanner/gl/buffer.h"
#include "iplanner/gl/vertex_array.h"
#include "iplanner/gl/program.h"
#include "iplanner/gl/gl_texture.h"
#include "iplanner/data/camera.h"
#include "iplanner/data/mesh.h"
#include "iplanner/data/light.h"
#include "iplanner/data/texture.h"
#include "iplanner/data/material.h"
#include "iplanner/object/ball.h"

namespace iplanner
{
class EngineWidget : public QOpenGLWidget
{
  Q_OBJECT

public:
  EngineWidget();
  ~EngineWidget();

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  void mousePressEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;

private:
  // Framebuffer rendering for mouse interaction
  void RenderMouseInteractionFramebuffer();

  // Mouse interaction object

  // Lattice
  double lattice_size_ = 1.;
  int lattice_num_ = 10;
  gl::ArrayBuffer<float> lattice_vbo_;
  gl::VertexArray lattice_vao_;

  // Ground depth buffer writing
  gl::ArrayBuffer<float> ground_depth_vbo_
  {
    -10.f, -10.f, 0.f,
     10.f, -10.f, 0.f,
    -10.f,  10.f, 0.f,
     10.f,  10.f, 0.f,
  };
  gl::ElementBuffer ground_depth_elements_{ 0, 1, 2, 3 };
  gl::VertexArray ground_depth_vao_;

  // Test mesh
  Mesh mesh_{ "..\\..\\fetch_ros\\fetch_description\\meshes\\base_link.dae" };
  gl::ArrayBuffer<float> mesh_vbo_positions_;
  gl::ArrayBuffer<float> mesh_vbo_normals_;
  gl::ArrayBuffer<float> mesh_vbo_tex_coords_;
  Texture<unsigned char> mesh_texture_;
  gl::TextureObject2D<unsigned char> mesh_texture_object_;
  gl::ElementBuffer mesh_elements_;
  gl::VertexArray mesh_texture_vao_;
  gl::VertexArray mesh_texture_light_vao_;
  gl::VertexArray mesh_color_light_vao_;
  GLenum mesh_texture_id_;

  // Test ball
  Ball ball_{ 3 };

  gl::Program color_3d_program_{ "..\\src\\shader", "color_3d"};
  gl::Program ground_depth_program_{ "..\\src\\shader", "ground_depth" };
  gl::Program mesh_texture_program_{ "..\\src\\shader", "mesh_texture" };
  gl::Program mesh_texture_light_program_{ "..\\src\\shader", "mesh_texture_light" };

  std::shared_ptr<Camera> camera_;
  std::vector<Light> lights_;
  Material material_;

  // UI
  bool draw_color_ = false;
  bool draw_light_ = true;

  // Mouse drag
  Vector2i mouse_last_pos_;
};
}

#endif // IPLANNER_WIDGET_ENGINE_WIDGET_H_
