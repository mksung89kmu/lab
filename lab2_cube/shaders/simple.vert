#version 430 core

layout(location=0) in vec3 coord3d;
layout(location=1) in vec3 v_color;


uniform mat4 model;  //4x4 mat
uniform mat4 view;  //4x4 mat
uniform mat4 projection;  //4x4 mat

out vec3 f_color;

void main(void)
{
   
   gl_Position = (projection * (view * (model * vec4( coord3d,1.0))));
   f_color = v_color;
}
