#version 440

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 frag_tex_coord;

void main()
{
  gl_Position = projection * view * model * vec4(pos, 1.f);
  frag_tex_coord = tex_coord;
}
