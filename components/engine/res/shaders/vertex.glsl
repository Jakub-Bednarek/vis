#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_textureCoords;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 v_frag_position;
out vec3 v_normal;
out vec2 v_textureCoords;

void main()
{
  gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);

  v_frag_position = in_position;
  v_normal = in_normal;
  v_textureCoords = in_textureCoords;
}
