#include "SceneHierarchyPanel.h"
#include "Core/Transform.h"
#include "Core/BoxCollider.h" 
#include <imgui/imgui.h>

void SceneHierarchyPanel::SetContext(std::vector<std::shared_ptr<GameObject>>* gameObjects)
{
    m_ContextObjects = gameObjects;
    m_SelectionContext = nullptr; // 切换场景时清空选中
}

void SceneHierarchyPanel::DrawEntityNode(GameObject* entity)
{
    if (!entity) return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (m_SelectionContext == entity) flags |= ImGuiTreeNodeFlags_Selected;

    bool nodeOpen = ImGui::TreeNodeEx((void*)entity, flags, "%s", entity->GetName().c_str());

    if (ImGui::IsItemClicked())
    {
        m_SelectionContext = entity;
    }

    if (nodeOpen)
    {
        for (auto childTransform : entity->transform->GetChildren()) {
            DrawEntityNode(childTransform->gameObject);
        }
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::OnImGuiRender()
{

    ImGui::Begin("Scene Hierarchy");

    if (m_ContextObjects)
    {
        for (auto& go : *m_ContextObjects)
        {
            DrawEntityNode(go.get());
        }
    }

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectionContext = nullptr; // 点击空白处取消选中

    ImGui::End();

    ImGui::Begin("Properties (Inspector)");
    if (m_SelectionContext != nullptr)
    {
        ImGui::Text("Name: %s", m_SelectionContext->GetName().c_str());
        ImGui::Separator();

        for (auto& comp : m_SelectionContext->GetComponents())
        {
            if (comp->enabled)
            {
                comp->OnInspectorGUI();  
                ImGui::Separator();
            }
        }
    }
    else
    {
        ImGui::TextDisabled("Select a node to view properties.");
    }
    ImGui::End();

}