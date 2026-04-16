#include "SceneHierarchyPanel.h"

void SceneHierarchyPanel::DrawNodeTree(BaseNode* node)
{
    if (!node) return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (m_SelectionContext == node) flags |= ImGuiTreeNodeFlags_Selected;

    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)node->id, flags, node->nodeName.c_str());

    if (ImGui::IsItemClicked())
    {
        m_SelectionContext = node;
    }

    if (nodeOpen)
    {
        for (BaseNode* child : node->children)
        {
            DrawNodeTree(child); // 递归画子节点
        }
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::OnImGuiRender()
{
    // 画层级面板 
    ImGui::Begin("Scene Hierarchy");
    DrawNodeTree(m_ContextNode);

    // 点击空白处取消选中
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectionContext = nullptr;

    ImGui::End();

    // 画属性面板 
    ImGui::Begin("Properties (Inspector)");
    if (m_SelectionContext != nullptr)
    {
        // 显示选中节点的名字
        ImGui::Text("Name: %s", m_SelectionContext->nodeName.c_str());
        ImGui::Separator();
        // 调用节点自己的多态函数画属性
        m_SelectionContext->OnImGuiRenderAttributes();
    }
    else
    {
        ImGui::Text("Select a node to view properties.");
    }
    ImGui::End();
}
