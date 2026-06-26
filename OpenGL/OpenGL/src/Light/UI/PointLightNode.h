#pragma once
#include "UI/BaseNode.h"
#include "Light/PointLight.h"

class  PointLightNode:public BaseNode
{
public:
    PointLight* m_TargetLight; // 实际光源的指

    PointLightNode(int id, std::string name, PointLight* light)
        : BaseNode(id, name), m_TargetLight(light)
    {
        // 初始化时，把 UI 的位置和光源的真实位置同步
        position = m_TargetLight->position;
    }

    // 重写多态函数
    void OnImGuiRenderAttributes() override
    {
        if (ImGui::DragFloat3("Position", &m_TargetLight->position.x, 0.1f)) {
            position = m_TargetLight->position; // 保持双向同步
        }

        // 画颜色
        ImGui::ColorEdit3("Color", &m_TargetLight->baseColor.x);

        // 画强度
        ImGui::SliderFloat("Intensity", &m_TargetLight->intensity, 0.0f, 5.0f);

        // 画衰减参数
        ImGui::DragFloat3("Attenuation (C/L/Q)", &m_TargetLight->attenuation.x, 0.01f);
    }

};
