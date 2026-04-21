#pragma once
#include "Core/Component.h"
#include "Core/Transform.h"
#include "Camera/Camera.h"
#include "Input.h"
#include <GLFW/glfw3.h> 
#include <memory>
#include "Core/BoxCollider.h"

class FirstPersonController:public Component
{
public:
    FirstPersonController(std::shared_ptr<Camera> camera);

    void Start() override;
    void Update(float deltaTime)override;

   std::shared_ptr<Camera>  GetCamera()const;

private:
	std::shared_ptr<Camera> m_Camera;
    float m_WalkSpeed = 3.0f;
    float m_RunSpeed = 6.0f;
    float m_MouseSensitivity = 0.1f;

    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;

    float m_LastMouseX = 0.0f;
    float m_LastMouseY = 0.0f;
    bool m_FirstMouse = true;

    bool CheckCollision();
};
