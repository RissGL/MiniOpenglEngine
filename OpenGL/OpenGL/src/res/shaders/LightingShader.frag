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
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuation;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 attenuation;//衰减，里面分别是常数项，一次项，二次项

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 attenuation;//衰减，里面分别是常数项，一次项，二次项

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

uniform Material u_Material;

uniform DirLight u_DirLight;
uniform PointLight u_PointLight;
uniform SpotLight u_SpotLight;

uniform Light u_Light;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //漫反射
    vec3 norm =normalize(normal);
    vec3 lightDir=normalize(-light.direction);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse =(diff*vec3(texture(u_Material.diffuse,TexCoords)))*light.diffuse;

    //镜面光
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=0.0;
    if(diff>0)
    {
        spec=pow(max(dot(viewDir,reflectDir),0.0),u_Material.shininess);
    }
    vec3 specular=vec3(texture(u_Material.specular, TexCoords))*spec*light.specular;
    vec3 result = (diffuse+specular);
    return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
    float distance=length(light.position-fragPos);
    float attenuation=1.0/(light.attenuation.x+light.attenuation.y*distance+light.attenuation.z*distance*distance);

    //漫反射
    vec3 norm =normalize(normal);
    vec3 lightDir=normalize(light.position - fragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse =(diff*vec3(texture(u_Material.diffuse,TexCoords)))*light.diffuse;

    //镜面光
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=0.0;
    if(diff>0)
    {
        spec=pow(max(dot(viewDir,reflectDir),0.0),u_Material.shininess);
    }
    vec3 specular=vec3(texture(u_Material.specular, TexCoords))*spec*light.specular;
    vec3 result = (diffuse+specular)*attenuation;
    return result;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir=normalize(light.position - fragPos);

    float theta =dot(lightDir,-normalize(light.direction));
    float epsilon=light.cutOff - light.outerCutOff;
    float intensity=clamp((theta-light.outerCutOff)/epsilon,0.0,1.0);

    float distance=length(light.position-fragPos);
    float attenuation=1.0/(light.attenuation.x+light.attenuation.y*distance+light.attenuation.z*distance*distance);

    //漫反射
    vec3 norm =normalize(Normal);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse =(diff*vec3(texture(u_Material.diffuse,TexCoords)))*light.diffuse;

    //镜面光
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=0.0;
    if(diff>0)
    {
        spec=pow(max(dot(viewDir,reflectDir),0.0),u_Material.shininess);
    }
    vec3 specular=vec3(texture(u_Material.specular, TexCoords))*spec*light.specular;
    vec3 result = (diffuse+specular)*attenuation*intensity;
    return result;
}

void main()
{
    vec3 norm =normalize(Normal);

    vec3 viewDir=normalize(u_ViewPos-FragPos);
    vec3 globalAmbient = vec3(texture(u_Material.diffuse, TexCoords)) * 0.2;
    vec3 result = vec3(0.0);

    result+=globalAmbient;
    /*result += CalcDirLight(u_DirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(u_PointLight, norm, FragPos, viewDir);
    }*/

    result+=CalcSpotLight(u_SpotLight,norm,FragPos,viewDir);

    //result+=vec3(texture(u_Material.emission,TexCoords)); 
    FragColor=vec4(result,1.0);
}