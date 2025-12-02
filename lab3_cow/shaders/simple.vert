#version 430 core

layout(location=0) in vec3 coord3d;
layout(location=1) in vec3 normal;

uniform mat4 model;  //4x4 mat
uniform mat4 view;  //4x4 mat
uniform mat4 projection;  //4x4 mat

out vec3 color;
void main(void)
{
   
   gl_Position = (projection * (view * (model * vec4( coord3d,1.0))));
   color  = (normal + vec3(1, 1, 1)) / 2.0f;
   //color = vec3(1,0,0);

}
