#version 440

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 frag_color;

void main()
{
  gl_Position = projection * view * model * vec4(pos, 1.f);
  frag_color = vec4(col, 1.f);
}
