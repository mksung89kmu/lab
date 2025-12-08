#version 430 core

layout(location=0) in vec3 coord3d;
layout(location=1) in vec3 normals;

uniform mat4 model;  //4x4 mat
uniform mat4 view;  //4x4 mat
uniform mat4 projection;  //4x4 mat
uniform mat3 normalMatrix; //transformation matrix for normals	

out vec3 position;
out vec3  normal;

void main(void)
{
   
   position = vec3(model * vec4(coord3d,1.0));
   normal = normalMatrix * normals;

   gl_Position = (projection * (view * (model * vec4( coord3d,1.0))));
}
