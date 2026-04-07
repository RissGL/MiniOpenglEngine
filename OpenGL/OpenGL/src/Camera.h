#pragma once
#include "glm/glm.hpp"

enum class ProjectionMode
{
	Perspective,  // 透视投影
	Orthographic  // 正交投影
};

class  Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = 0.0f);
	~ Camera();

	glm::vec3 GetCameraPos() const;
	glm::vec3 GetCameraTarget() const;
	glm::vec3 GetCameraFront() const;
	glm::vec3 GetCameraUp() const;

	ProjectionMode GetProjectionMode() const { return m_ProjectionMode; }
	float GetFov() const { return m_Fov; }
	float GetAspectRatio() const { return m_AspectRatio; }
	float GetNearPlane() const { return m_NearPlane; }
	float GetFarPlane() const { return m_FarPlane; }
	float GetOrthoWidth() const { return m_OrthoScale; }

	void SetCameraPos(glm::vec3 targetPos);
	void SetCameraFront(glm::vec3 targetFront);
	void MoveCamera(glm::vec3 vector);

	void SetProjectionMode(ProjectionMode mode) { m_ProjectionMode = mode; }
	void SetFOV(float fov) { m_Fov = fov; }
	void SetAspectRatio(float ratio) { m_AspectRatio = ratio; }
	void SetClipPlanes(float nearClip, float farClip) { m_NearPlane= nearClip; m_FarPlane = farClip; }
	void SetOrthoWidth(float width) { m_OrthoScale= width; }

	void UpdateCameraVectors();

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch=true);

	//得到视图矩阵
	glm::mat4 GetViewMatrix() const;
	//得到投影矩阵
	glm::mat4 GetProjectionMatrix() const;

private:
	// 摄像机的空间属性
	glm::vec3 m_Pos;
	glm::vec3 m_Front; 
	glm::vec3 m_CameraUp;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	// 欧拉角 
	float m_Yaw;//偏航角
	float m_Pitch;// 俯仰角
	float m_MouseSensitivity;// 鼠标灵敏度

	// 其他属性
	ProjectionMode m_ProjectionMode; // 投影模式
	float m_Fov; // 视野角度
	float m_AspectRatio; // 宽高比
	float m_NearPlane; // 近平面距离
	float m_FarPlane; // 远平面距离
	float m_OrthoScale; // 正交投影缩放因子
};

