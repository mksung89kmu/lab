#version 400

out  vec4 FragColors;
in vec3 color;

void main()
{
   FragColors = vec4(color, 1.0);
}