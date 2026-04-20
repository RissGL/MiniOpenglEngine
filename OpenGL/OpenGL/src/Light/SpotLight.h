#pragma once
#include "BaseLight.h"

class SpotLight:public BaseLight
{
public:
	glm::vec3 position;//位置
	glm::vec3 direction;//方向
	glm::vec3 attenuation;//衰减
	float cutOff;//角度,内部自动转化成余弦值
	float outerCutOff;//外切角

	SpotLight(glm::vec3 position=glm::vec3(0.0f), glm::vec3 attenuation=glm::vec3(1.0f, 0.09f, 0.032f),
		glm::vec3 diretion=glm::vec3(0.0f,0.0f,1.0f),glm::vec3 color=glm::vec3(1.0f), 
		float cutOff=45.0f, float outerCutOff=50.0f,float intensityfloat=1.0f);

	void BindToShader(Shader& shader, const std::string& name) const override;

private:

};