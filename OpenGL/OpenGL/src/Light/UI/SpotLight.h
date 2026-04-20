#pragma once
#include "UI/BaseNode.h"
#include "Light/SpotLight.h"

class  SpotLightNode :public BaseNode
{
public:
    SpotLight* m_TargetLight; // 实际光源的指
    glm::vec3 direction;

    SpotLightNode(int id, std::string name, SpotLight* light)
        : BaseNode(id, name), m_TargetLight(light)
    {
        // 初始化时，把 UI 的位置和光源的真实位置同步
        position = m_TargetLight->position;
        direction = m_TargetLight->position;
    }

    // 重写多态函数
    void OnImGuiRenderAttributes() override
    {
        if (ImGui::DragFloat3("Position", &m_TargetLight->position.x, 0.1f)) {
            position = m_TargetLight->position; // 保持双向同步
        }
        if (ImGui::DragFloat3("Diretion", &m_TargetLight->direction.x, 0.1f)) {
            direction = m_TargetLight->direction; // 保持双向同步
        }

        // 画颜色
        ImGui::ColorEdit3("Color", &m_TargetLight->baseColor.x);

        // 画强度
        ImGui::SliderFloat("Intensity", &m_TargetLight->intensity, 0.0f, 5.0f);

        // 画角度
        ImGui::SliderFloat("CutOff", &m_TargetLight->cutOff, 0.0f, 180.0f);
        // 画角度
        ImGui::SliderFloat("OuterCutOff", &m_TargetLight->outerCutOff, 0.0f, 180.0f);


        // 画衰减参数
        ImGui::DragFloat3("Attenuation (C/L/Q)", &m_TargetLight->attenuation.x, 0.01f);
    }

};
