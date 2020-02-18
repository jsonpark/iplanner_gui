#version 440

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 model_inv_tp;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_tex_coord;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.f);
  vec4 frag_homo_position = model * vec4(position, 1.f);
  frag_position = frag_homo_position.xyz / frag_homo_position.w;
  frag_normal = model_inv_tp * normal;
  frag_tex_coord = tex_coord;
}
