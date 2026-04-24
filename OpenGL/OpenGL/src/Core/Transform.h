#pragma once
#include "Core/Component.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Transform :public Component
{
public:
	glm::vec3 localPosition=glm::vec3(0.0f);
	glm::vec3 localEulerAngles=glm::vec3(0.0f);
	glm::vec3 localScale=glm::vec3(1.0f);

	Transform* parent = nullptr;
	std::vector<Transform*> children;

	~Transform()override = default;

	void SetParent(Transform* newParent);

	glm::mat4 GetModelMatrix() const;

	glm::vec3 GetWorldPosition() const;

	std::vector<Transform*> GetChildren()const;
	void RemoveChild(Transform* child);
	void OnInspectorGUI() override;
private:

};