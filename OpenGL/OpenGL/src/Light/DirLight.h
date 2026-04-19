#pragma once
#include "BaseLight.h"

class DirLight :public BaseLight
{
public:
	DirLight(glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f),glm::vec3 color= glm::vec3(1.0f),float intensity = 1.0f);

	glm::vec3 lightDirection;
	void BindToShader(Shader& shader, const std::string& name)const override;

private:

};
