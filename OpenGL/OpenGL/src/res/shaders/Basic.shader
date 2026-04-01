#shader vertex
#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
uniform float u_XOffset;
uniform mat4 u_MVP;
out vec2 v_TexCoord;

void main()
{
    //gl_Position = vec4(position * 0.5, 0.0, 1.0);
    gl_Position = u_MVP*(vec4(position.x, position.y, 0.0, 1.0));
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color; 

in vec2 v_TexCoord;
uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
    vec4 texCoord=texture(u_Texture,v_TexCoord);
    color = texCoord;
}