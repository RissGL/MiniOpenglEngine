#pragma once
#include "glm/glm.hpp"
#include <string>
#include "Base//Shader.h"

class BaseLight
{
public:
	glm::vec3 baseColor; // 光源本色
	float intensity;     // 光源强度

	BaseLight(glm::vec3 color,float intensity) 
		:baseColor(color),intensity(intensity)
	{
	};

	virtual ~BaseLight() {}

	virtual void BindToShader(Shader& shader, const std::string& name)const = 0 ;
protected:
	virtual glm::vec3 GetAmbient() const { return baseColor * 0.1f; }
	virtual glm::vec3 GetDiffuse() const { return baseColor * intensity; }
	virtual glm::vec3 GetSpecular() const { return baseColor * intensity; }
};
