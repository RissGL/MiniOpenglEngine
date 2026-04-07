#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"


Camera::Camera(glm::vec3 pos, glm::vec3 targetFront)
	:m_Pos(pos), m_Front(targetFront)
{
}

Camera::~Camera()
{
}

glm::vec3 Camera::GetCameraPos()const
{
	return m_Pos;
}

glm::vec3 Camera::GetCameraTarget() const
{
	return m_Pos+m_Front;
}

void Camera::SetCameraPos(glm::vec3 targetPos)
{
	m_Pos = targetPos;
}

void Camera::SetCameraFront(glm::vec3 targetFront)
{
	m_Front = targetFront;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Pos, m_Front+m_Pos, glm::vec3(0, 1, 0));
}