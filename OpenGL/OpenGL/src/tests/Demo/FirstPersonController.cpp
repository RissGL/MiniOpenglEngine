#include "FirstPersonController.h"

FirstPersonController::FirstPersonController(std::shared_ptr<Camera> camera)
	:m_Camera(camera)
{
}

void FirstPersonController::Start()
{
    GLFWwindow* window = glfwGetCurrentContext();
    /*if (window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }*/
}

std::shared_ptr<Camera> FirstPersonController::GetCamera()const
{
    return m_Camera;
}

bool FirstPersonController::CheckCollision()
{
    BoxCollider* myCollider = gameObject->GetComponent<BoxCollider>();
    if (!myCollider) return false;

    // 遍历场景里所有的墙壁怪物
    for (BoxCollider* other : BoxCollider::s_AllColliders)
    {
        if (myCollider->IsOverlapping(other)) {
            return true; // 撞到了
        }
    }
    return false; // 没撞到，安全
}

void FirstPersonController::Update(float deltaTime)
{
    if (!enabled || !m_Camera) return;

    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
    }

    float mouseX = Input::GetMouseX();
    float mouseY = Input::GetMouseY();

    if (m_FirstMouse) {
        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;
        m_FirstMouse = false;
    }

    float xOffset = (mouseX - m_LastMouseX) * m_MouseSensitivity;
    float yOffset = (m_LastMouseY - mouseY) * m_MouseSensitivity; 

    m_LastMouseX = mouseX;
    m_LastMouseY = mouseY;

    m_Yaw += xOffset;
    m_Pitch += yOffset;

    if (m_Pitch > 89.0f) m_Pitch = 89.0f;
    if (m_Pitch < -89.0f) m_Pitch = -89.0f;

    transform->localEulerAngles = glm::vec3(m_Pitch, m_Yaw, 0.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front = glm::normalize(front);

    glm::vec3 moveFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    glm::vec3 moveRight = glm::normalize(glm::cross(moveFront, glm::vec3(0.0f, 1.0f, 0.0f)));

    float currentSpeed = Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) ? m_RunSpeed : m_WalkSpeed;
    float velocity = currentSpeed * deltaTime;

    glm::vec3 moveVelocity(0.0f);
    if (Input::IsKeyPressed(GLFW_KEY_W)) moveVelocity += moveFront * velocity;
    if (Input::IsKeyPressed(GLFW_KEY_S)) moveVelocity -= moveFront * velocity;
    if (Input::IsKeyPressed(GLFW_KEY_A)) moveVelocity -= moveRight * velocity;
    if (Input::IsKeyPressed(GLFW_KEY_D)) moveVelocity += moveRight * velocity;


    // 先只尝试在 X 轴移动
    transform->localPosition.x += moveVelocity.x;
    if (CheckCollision()) {
        transform->localPosition.x -= moveVelocity.x; 
    }

    // 再尝试在 Z 轴移动
    transform->localPosition.z += moveVelocity.z;
    if (CheckCollision()) {
        transform->localPosition.z -= moveVelocity.z; 
    }

    transform->localPosition.y = 1.5f;

    m_Camera->SetCameraPos (transform->localPosition);


    m_Camera->SetCameraFront (front);
}
