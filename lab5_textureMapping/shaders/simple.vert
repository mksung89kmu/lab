#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 textureCoord;

out vec3 normal;
out vec3 pos;
out vec2 coord;


uniform mat3 NormalMatrix; 
uniform mat4 MVP;
uniform mat4 Model;

void main()
{
   coord = textureCoord;
   vec3 N = normalize( NormalMatrix * VertexNormal);
   normal = N;

   vec4 worldCoords = Model * vec4(VertexPosition,1.0);
   pos = worldCoords.xyz;
 
   gl_Position = MVP * vec4(VertexPosition,1.0);
   
}