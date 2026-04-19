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
            DrawNodeTree(child); // 递归画子节点
        }
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::OnImGuiRender()
{
    float panelWidth = 280.0f;

    // 固定层级面板 (Scene Hierarchy)
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panelWidth, (float)MyWindow::GetHeight()), ImGuiCond_Always);
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    // 画层级面板 
    ImGui::Begin("Scene Hierarchy");
    DrawNodeTree(m_ContextNode);

    // 点击空白处取消选中
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectionContext = nullptr;

    ImGui::End();

    // 右侧：固定属性面板 (Properties)

    ImGui::SetNextWindowPos(ImVec2((float)MyWindow::GetWidth() - panelWidth, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panelWidth, (float)MyWindow::GetHeight()), ImGuiCond_Always);

    ImGui::Begin("Properties (Inspector)", nullptr, windowFlags);
    if (m_SelectionContext != nullptr)
    {
        ImGui::Text("Name: %s", m_SelectionContext->nodeName.c_str());
        ImGui::Separator();

        m_SelectionContext->OnImGuiRenderAttributes();
    }
    else
    {
        ImGui::TextDisabled("Select a node to view properties."); // 使用 TextDisabled 让未选中时的提示文字变灰
    }
    ImGui::End();
}
