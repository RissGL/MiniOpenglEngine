#pragma once
#pragma once
#include "UI/BaseNode.h"
#include "Light/DirLight.h"

class  DirLightNode :public BaseNode
{
public:
    DirLight* m_TargetLight; // 实际光源的指

    DirLightNode(int id, std::string name, DirLight* light)
        : BaseNode(id, name), m_TargetLight(light)
    {
        // 初始化时，把 UI 的位置和光源的真实位置同步
        lightDirection = m_TargetLight->lightDirection;
    }

    // 重写多态函数
    void OnImGuiRenderAttributes() override
    {
        if (ImGui::DragFloat3("Position", &m_TargetLight->lightDirection.x, 0.1f))
        {
            lightDirection = m_TargetLight->lightDirection; // 保持双向同步
        }

        // 画颜色
        ImGui::ColorEdit3("Color", &m_TargetLight->baseColor.x);

        // 画强度
        ImGui::SliderFloat("Intensity", &m_TargetLight->intensity, 0.0f, 5.0f);
    }

private:
    glm::vec3 lightDirection;
};

