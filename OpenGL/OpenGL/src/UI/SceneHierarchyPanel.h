#pragma once
#include "UI/BaseNode.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel(BaseNode* rootNode) : m_ContextNode(rootNode) {}

	void OnImGuiRender();

private:
	BaseNode* m_ContextNode;
	BaseNode* m_SelectionContext=nullptr;//当前选中节点

	void DrawNodeTree(BaseNode* node);
};
