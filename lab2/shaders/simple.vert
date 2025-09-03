#version 450 

layout(location = 0) in vec4 coord3d;
layout(location = 1) in vec3 v_color;

out vec3 f_color;

void main()
{
 
   gl_Position =  coord3d;
   f_color = v_color;

}
