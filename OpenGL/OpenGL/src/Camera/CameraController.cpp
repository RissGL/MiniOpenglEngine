#include "Window/MyWindow.h"
#include "CameraController.h"


CameraController::CameraController(Camera& camera,CameraType type)
    : m_Camera(camera), m_MoveSpeed(2.0f), m_PanSpeed(0.01f),
    m_FirstMouse(true), m_LastX(0.0f), m_LastY(0.0f),
    cameraType(type)
{
}

void CameraController::OnUpdate(float deltaTime)
{
    //  键盘 W/A/S/D 移动
    if (Input::IsKeyPressed(GLFW_KEY_W)) m_Camera.MoveCamera(m_MoveSpeed * m_Camera.GetCameraFront() * deltaTime);
    if (Input::IsKeyPressed(GLFW_KEY_S)) m_Camera.MoveCamera(-m_MoveSpeed * m_Camera.GetCameraFront() * deltaTime);
    if (Input::IsKeyPressed(GLFW_KEY_A)) m_Camera.MoveCamera(-glm::normalize(glm::cross(m_Camera.GetCameraFront()
        , m_Camera.GetCameraUp())) * m_MoveSpeed * deltaTime);
    if (Input::IsKeyPressed(GLFW_KEY_D)) m_Camera.MoveCamera(glm::normalize(glm::cross(m_Camera.GetCameraFront()
        , m_Camera.GetCameraUp())) * m_MoveSpeed * deltaTime);

    // 鼠标滚轮缩放
    float scrollY = Input::GetScrollY();
    if (scrollY != 0.0f) {
        m_Camera.ProcessMouseScroll(scrollY);
    }

    OnMouseUpdate();
}
void CameraController::OnMouseUpdate() 
{
    float currentX = Input::GetMouseX();
    float currentY = Input::GetMouseY();

    if (m_FirstMouse) { m_LastX = currentX; m_LastY = currentY; m_FirstMouse = false; }

    float xoffset = currentX - m_LastX;
    float yoffset = m_LastY - currentY; // 如果是相反的手感，把 currentY 和 m_LastY 调换一下
    m_LastX = currentX;
    m_LastY = currentY;

    // 右键旋转
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) && (xoffset != 0 || yoffset != 0)) {
        m_Camera.ProcessMouseMovement(xoffset, yoffset);
    }
    // 中键平移
    else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE) && (xoffset != 0 || yoffset != 0)) {
        m_Camera.ProcessMousePan(xoffset, yoffset);
    }

    // 如果没有任何按键按下，重置 FirstMouse 状态，防止下次点击时相机跳跃
    if (!Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) && !Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        m_FirstMouse = true;
    }
}

void CameraController::SetCmeraSpeed(float newSpeed)
{
    m_MoveSpeed = newSpeed;
}

float CameraController::GetCameraMoveSpeed() const
{
    return m_MoveSpeed;
}

Camera& CameraController::GetCamera() const
{
    return m_Camera;
}
