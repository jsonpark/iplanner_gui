#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 col;

uniform mat4 matrix;

out vec4 frag_color;

void main()
{
  gl_Position = matrix * vec4(pos, 0.f, 1.f);
  frag_color = vec4(col, 1.f);
}
