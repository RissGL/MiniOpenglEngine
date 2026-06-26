#include "DirLight.h"
#include "Base/Shader.h"

DirLight::DirLight(glm::vec3 direction, glm::vec3 color, float intensity)
    :BaseLight(color,intensity),lightDirection(direction)
{
}

void DirLight::BindToShader(Shader& shader, const std::string& name) const
{
    shader.SetUniform3f(name + ".direction", lightDirection.x, lightDirection.y, lightDirection.z);

    glm::vec3 ambient = GetAmbient();
    glm::vec3 diffuse = GetDiffuse();
    glm::vec3 specular = GetSpecular();

    shader.SetUniform3f(name + ".ambient",  ambient.x,  ambient.y,  ambient.z);
    shader.SetUniform3f(name + ".diffuse",  diffuse.x,  diffuse.y,  diffuse.z);
    shader.SetUniform3f(name + ".specular",  specular.x,  specular.y,  specular.z);
}
