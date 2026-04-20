#include "Transform.h"

void Transform::SetParent(Transform* newParent)
{
	if (parent!=nullptr)
	{
		parent->RemoveChild(this);
	}
	parent = newParent;
	if (parent!=nullptr) 
	{
		parent->children.push_back(this);
	}
}

glm::mat4 Transform::GetModelMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);

	if (parent != nullptr) 
	{
		model = parent->GetModelMatrix();
	}

	model = glm::translate(model, localPosition);
	model = glm::rotate(model, glm::radians(localEulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(localEulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(localEulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, localScale);

	return model;
}

glm::vec3 Transform::GetWorldPosition() const
{
	return glm::vec3(GetModelMatrix()[3]); // 从矩阵的第四列提取位置
}

void Transform::RemoveChild(Transform* child)
{
	auto it = std::remove(children.begin(), children.end(), child);

	children.erase(it, children.end());
}
