#shader vertex
#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    //gl_Position = vec4(position * 0.5, 0.0, 1.0);
    gl_Position = u_Projection*u_View*u_Model*(vec4(position.x,position.y,position.z, 1.0));
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec3 u_LightColor;
uniform float u_LightIntensity;

void main()
{
    FragColor=vec4(u_LightColor,u_LightIntensity);
}