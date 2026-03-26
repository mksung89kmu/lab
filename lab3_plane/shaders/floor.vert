#version 400

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexColor;

uniform mat4 mvp;
out vec3 f_color;

void main(void)
{

  gl_Position = mvp * VertexPosition;
  f_color = VertexColor;

}
