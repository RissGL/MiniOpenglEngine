#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 attenuation, glm::vec3 diretion,
    glm::vec3 color, float cutOff,float outerCutOff, float intensity)
	:BaseLight(color,intensity),position(position),direction(diretion),
    attenuation(attenuation), cutOff(cutOff), outerCutOff(outerCutOff)
{
}

void SpotLight::BindToShader(Shader& shader, const std::string& name) const
{
    shader.SetUniform3f(name + ".position", position.x, position.y, position.z);
    shader.SetUniform3f(name + ".direction", direction.x, direction.y, direction.z);

    shader.SetUniform1f(name + ".cutOff", glm::cos(glm::radians(cutOff)));
    shader.SetUniform1f(name + ".outerCutOff", glm::cos(glm::radians(outerCutOff)));

    glm::vec3 ambient = GetAmbient();
    glm::vec3 diffuse = GetDiffuse();
    glm::vec3 specular = GetSpecular();

    shader.SetUniform3f(name + ".ambient", ambient.x, ambient.y, ambient.z);
    shader.SetUniform3f(name + ".diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader.SetUniform3f(name + ".specular", specular.x, specular.y, specular.z);

    shader.SetUniform3f(name + ".attenuation", attenuation.x, attenuation.y, attenuation.z);

}
