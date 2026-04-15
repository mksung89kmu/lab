#version 400


layout( location = 0 ) out vec4 FragColor;


in vec3 normal;
in vec3 pos;


uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 camPos;
uniform float shiness;

const vec3 lightPos = vec3(5,5,5);  //bunny is huge, so the light is far away
const vec3 lightIntensity = vec3(1.0,1.0,1.0);

void main() 
{

   vec3 finalColor = vec3(0,0,0);
   vec3 ambient = lightIntensity * Ka;
     
   vec3 N = normalize(normal);
   vec3 L = normalize(lightPos - pos);
   vec3 V = normalize(camPos - pos);
   vec3 H = normalize(L + V);

   vec3 diffuse = lightIntensity * Kd * max(dot(L,N),0.0);
   vec3 specular = lightIntensity * pow(max(dot(H,N),0.0f), shiness);

   vec3 ambient_diffuse = ambient + diffuse;
   FragColor = vec4(ambient_diffuse + specular,1.0);

 }
