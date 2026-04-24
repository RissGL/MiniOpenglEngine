#pragma once
#include "Core/GameObject.h"
#include <vector>
#include <memory>

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;

	void SetContext(std::vector<std::shared_ptr<GameObject>>* gameObjects);

	void OnImGuiRender();

private:
	std::vector<std::shared_ptr<GameObject>>* m_ContextObjects = nullptr;
	GameObject* m_SelectionContext = nullptr; // 当前选中的物体

	void DrawEntityNode(GameObject* entity);
};