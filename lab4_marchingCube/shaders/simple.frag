#version 400

out  vec4 FragColors;

in vec3 pos;  //position in camera coordinates
in vec3 norm;

// compute the phong shading

const vec3 lightPos = vec3(10,10,10);
const vec3 lightColor = vec3(1,1,1);
const vec3 objectColor= vec3(0.5,0,0);	

uniform mat4 view;  //4x4 mat

void main()
{
    vec3 lp = vec3(view * vec4(lightPos,1.0)); //light position in camera coordinates

    vec3 diffuse = max(dot(norm, lp), 0.0) * lightColor * objectColor;
    vec3 ambient = 0.01 * lightColor * objectColor;

    vec3 color = (ambient );
    FragColors = vec4(color, 1.0);
}