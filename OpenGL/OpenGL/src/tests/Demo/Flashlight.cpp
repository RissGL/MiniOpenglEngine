#include "Flashlight.h"

Flashlight::Flashlight(std::shared_ptr<SpotLight> light, Shader& shader,const std::string& shaderName)
	:spotLight(light), m_Shader(shader),m_ShaderName(shaderName)
{
}

Flashlight::~Flashlight()
{
}

void  Flashlight::Update(float deltaTime)
{
    if (!enabled || !spotLight) return;

    if (Input::IsKeyPressed(GLFW_KEY_F))
    {
        m_IsOn = !m_IsOn;
        std::cout << (m_IsOn ? "手电筒开启" : "手电筒关闭") << std::endl;
    }

    if (m_IsOn)
    {
        m_Battery -= 0.3f * deltaTime; // 每秒掉 1 点电
        if (m_Battery <= 0.0f) {
            m_IsOn = false; // 没电了强制关机！
            m_Battery = 0.0f;
        }
    }

    spotLight->position = transform->GetWorldPosition();

    spotLight->intensity = m_IsOn ? 1.0f : 0.0f;
    m_Shader.Bind();
    spotLight->BindToShader(m_Shader, m_ShaderName);
}