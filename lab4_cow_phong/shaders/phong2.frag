#version 400

uniform vec3 lightPos; //light position in world space
uniform vec3 viewPos;  //camera position in world space

out vec4 FragColors;
in vec3 position;
in vec3 normal;

const vec3 ambientColor = vec3(0.1,0.1,0);
const vec3 diffuseColor = vec3(0.7,0.7,0.0);
const vec3 specularColor = vec3(1.0,1.0,0.0);	
const float shininess = 32.0;
const vec3 lightColor = vec3(1.0,1.0,1.0);


void main()
{
   vec3 L = normalize(lightPos - position);
   vec3 V = normalize(viewPos - position);	
   vec3 N = normal;
   vec3 R = reflect(-L, N);

   vec3 ambient = ambientColor * lightColor;	
   vec3 diffuse = max(dot(N,L),0.0) * lightColor * diffuseColor;
   vec3 specular = pow( max(dot(R,V),0.0), shininess) * lightColor * specularColor;
   vec3 color = diffuse + specular + ambient;   

   FragColors = vec4(color, 1.0);
}