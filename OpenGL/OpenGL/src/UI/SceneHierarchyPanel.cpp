#include "SceneHierarchyPanel.h"
#include "Window/MyWindow.h"

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
            DrawNodeTree(child); // µÝ¹é»­×Ó½Úµã
        }
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");
    DrawNodeTree(m_ContextNode);

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectionContext = nullptr;

    ImGui::End();

    ImGui::Begin("Properties (Inspector)");
    if (m_SelectionContext != nullptr)
    {
        ImGui::Text("Name: %s", m_SelectionContext->nodeName.c_str());
        ImGui::Separator();
        m_SelectionContext->OnImGuiRenderAttributes();
    }
    else
    {
        ImGui::TextDisabled("Select a node to view properties.");
    }
    ImGui::End();
}
