#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 attenuation,glm::vec3 color, float intensity)
	:BaseLight(color,intensity),position(position),attenuation(attenuation)
{
}

void PointLight::BindToShader(Shader& shader, const std::string& name) const
{
    shader.SetUniform3f(name + ".position", position.x, position.y, position.z);

    glm::vec3 ambient = GetAmbient();
    glm::vec3 diffuse = GetDiffuse();
    glm::vec3 specular = GetSpecular();

    shader.SetUniform3f(name + ".ambient",  ambient.x,  ambient.y,  ambient.z);
    shader.SetUniform3f(name + ".diffuse",  diffuse.x,  diffuse.y,  diffuse.z);
    shader.SetUniform3f(name + ".specular",  specular.x,  specular.y,  specular.z);

    shader.SetUniform3f(name + ".attenuation", attenuation.x,attenuation.y, attenuation.z);
}