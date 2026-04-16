#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;

void main()
{
    vec3 result = u_LightColor * u_ObjectColor;
    FragColor=vec4(result,1.0);
}