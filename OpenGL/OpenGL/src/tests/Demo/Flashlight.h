#pragma once
#include <GL/glew.h>
#include "Input.h" 
#include "Core/Component.h"
#include "Core/Transform.h"
#include "Light/SpotLight.h" 
#include <memory>
#include <iostream>

class Flashlight:public Component
{
public:
	Flashlight(std::shared_ptr<SpotLight> light, Shader& shader, const std::string& shaderName);
	~Flashlight();

	void Update(float deltaTime) override;
private:
	std::shared_ptr<SpotLight> spotLight;
	bool m_IsOn = true;                     
	float m_Battery = 100.0f;

	Shader m_Shader;
	std::string m_ShaderName;
};
