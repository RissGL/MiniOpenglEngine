#version 330 core

out vec4 FragColor;

uniform vec3 u_LightColor;
uniform float u_LightIntensity;

void main()
{
    FragColor=vec4(u_LightColor,u_LightIntensity);
}