#pragma once
#include "BaseLight.h"

class  PointLight:public BaseLight
{
public:
	glm::vec3 position;
	glm::vec3 attenuation;

	 PointLight(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 attenuation=glm::vec3(1.0f, 0.09f, 0.032f),
		 glm::vec3 color = glm::vec3(1.0f), float intensity = 1.0f);

	 void BindToShader(Shader& shader, const std::string& name) const override;
private:

};

