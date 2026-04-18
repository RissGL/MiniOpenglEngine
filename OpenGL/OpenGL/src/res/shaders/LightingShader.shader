#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoords;


uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 Normal;
out vec3 FragPos;  
out vec2 TexCoords;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
    FragPos=vec3(u_Model*vec4(aPos,1.0));
    Normal=mat3(transpose(inverse(u_Model)))*aNormal;
    TexCoords=aTexCoords;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 u_ViewPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 lightPos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Material u_Material;
uniform Light u_Light;


void main()
{
    //环境光
    vec3 ambient=vec3(texture(u_Material.diffuse,TexCoords))*u_Light.ambient;

    //漫反射
    vec3 norm =normalize(Normal);
    vec3 lightDir=normalize(u_Light.lightPos-FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse =(diff*vec3(texture(u_Material.diffuse,TexCoords)))*u_Light.diffuse;

    //镜面光
    vec3 viewDir=normalize(u_ViewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);

    float spec=0.0;
    if(diff>0)
    {
        spec=pow(max(dot(viewDir,reflectDir),0.0),u_Material.shininess);
    }

    vec3 specular=vec3(texture(u_Material.specular, TexCoords))*spec*u_Light.specular;

    vec3 emission=vec3(texture(u_Material.emission,TexCoords)); 

    vec3 result = (ambient+diffuse+specular+emission);
    FragColor=vec4(result,1.0);
}