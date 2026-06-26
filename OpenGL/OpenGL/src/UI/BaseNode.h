#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "imgui/imgui.h"

class  BaseNode
{
public:
	int id;
	std::string nodeName;
	std::vector<BaseNode*> children;
	glm::vec3 position = glm::vec3(0.0f);

	BaseNode(int id, std::string& name) :id(id), nodeName(name) {}
	virtual ~BaseNode()
	{

	}

	void AddChild(BaseNode* child) { children.push_back(child); }

	virtual void OnImGuiRenderAttributes()
	{
		// 默认的基础属性
		ImGui::DragFloat3("Position", &position.x, 0.1f);
	}
};
