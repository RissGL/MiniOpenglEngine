#include "LightComponents.h"
#include <imgui/imgui.h>

void PointLightComponent::OnInspectorGUI()
{
    ImGui::Text("Point Light");
    if (lightData)
    {
        ImGui::ColorEdit3("Base Color", &lightData->baseColor.x);
        ImGui::DragFloat("Intensity", &lightData->intensity, 0.01f, 0.0f, 100.0f);
    }
}

void DirLightComponent::OnInspectorGUI()
{
    ImGui::Text("Directional Light");
    if (lightData)
    {
        ImGui::ColorEdit3("Color", &lightData->baseColor.x);
        ImGui::DragFloat("Intensity", &lightData->intensity, 0.01f, 0.0f, 100.0f);
    }
}

void SpotLightComponent::OnInspectorGUI()
{
    ImGui::Text("Spot Light");
    if (lightData)
    {
        ImGui::ColorEdit3("Base Color", &lightData->baseColor.x);
        ImGui::DragFloat("Intensity", &lightData->intensity, 0.01f, 0.0f, 100.0f);
        ImGui::DragFloat("Cutoff", &lightData->cutOff, 0.1f, 0.0f, 90.0f);
        ImGui::DragFloat("Outer Cutoff", &lightData->outerCutOff, 0.1f, 0.0f, 90.0f);
    }
}