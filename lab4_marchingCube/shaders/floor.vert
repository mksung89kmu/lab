#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 mvp;
out vec3 f_color;

void main(void)
{

  gl_Position = mvp * vec4(VertexPosition,1.0);
  
  f_color  = (VertexNormal + vec3(1, 1, 1)) / 2.0f;

}
