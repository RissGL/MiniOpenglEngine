#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

#include "MyTime.h"
#include "Window/MyWindow.h"

Camera::Camera(glm::vec3 position, float yaw, float pitch)
    : m_Pos(position),
    m_Yaw(yaw),
    m_Pitch(pitch),
    m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_CameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_Right(glm::vec3(1.0f, 0.0f, 0.0f)),
    m_MouseSensitivity(0.07f),
    m_ProjectionMode(ProjectionMode::Perspective),
    m_Fov(45.0f),
    m_AspectRatio(1920.0f / 1080.0f),
    m_NearPlane(0.1f),
    m_FarPlane(100.0f),
    m_OrthoScale(10.0f)
{
	m_AspectRatio = MyWindow::GetWidth() / (float)MyWindow::GetHeight();
	UpdateCameraVectors();
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

glm::vec3 Camera::GetCameraFront() const
{
	return m_Front;
}

glm::vec3 Camera::GetCameraUp() const
{
	return m_CameraUp;
}

void Camera::SetCameraPos(glm::vec3 targetPos)
{
	m_Pos = targetPos;
}

void Camera::SetCameraFront(glm::vec3 targetFront)
{
	m_Front = targetFront;
}

void Camera::MoveCamera(glm::vec3 vector)
{
	m_Pos += vector;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;

	front.y = sin(glm::radians(m_Pitch));
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_CameraUp = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= m_MouseSensitivity;
	yOffset *= m_MouseSensitivity;

	m_Pitch += yOffset;
	m_Yaw += xOffset;

	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	m_Fov -= (float)yOffset;
	if (m_Fov < 1.0f)
		m_Fov = 1.0f;
	if (m_Fov > 45.0f)
		m_Fov = 45.0f;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Pos, m_Front+m_Pos, m_CameraUp);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearPlane, m_FarPlane);
}
