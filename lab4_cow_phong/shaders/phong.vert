#version 430 core

layout(location=0) in vec3 coord3d;
layout(location=1) in vec3 normal;

uniform mat4 model;  //4x4 mat
uniform mat4 view;  //4x4 mat
uniform mat4 projection;  //4x4 mat
uniform mat3 normalMatrix; //transformation matrix for normals	


uniform vec3 lightPos; //light position in world space
uniform vec3 viewPos;  //camera position in world space


const vec3 ambientColor = vec3(0.1,0.1,0);
const vec3 diffuseColor = vec3(0.7,0.7,0.0);
const vec3 specularColor = vec3(1.0,1.0,0.0);	
const float shininess = 32.0;
const vec3 lightColor = vec3(1.0,1.0,1.0);

out vec3 color;
void main(void)
{
   

   gl_Position = (projection * (view * (model * vec4( coord3d,1.0))));
   vec3 position = vec3(model * vec4(coord3d,1.0));

   vec3 L = normalize(lightPos - position);
   vec3 V = normalize(viewPos - position);	
   vec3 N = normalize(normalMatrix * normal);
   vec3 R = reflect(-L, N);

   vec3 ambient = ambientColor * lightColor;	
   vec3 diffuse = max(dot(N,L),0.0) * lightColor * diffuseColor;
   vec3 specular = pow( max(dot(R,V),0.0), shininess) * lightColor * specularColor;
   color = diffuse + specular + ambient;   

}
