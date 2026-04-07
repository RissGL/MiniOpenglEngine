#pragma once
#include "glm/glm.hpp"

class  Camera
{
public:
	 Camera(glm::vec3 pos,glm::vec3 target);
	~ Camera();

	glm::vec3 GetCameraPos() const;
	glm::vec3 GetCameraTarget() const;
	glm::vec3 GetCameraFront() const;

	void SetCameraPos(glm::vec3 targetPos);
	void SetCameraFront(glm::vec3 targetFront);

	glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 m_Pos;
	glm::vec3 m_Front; 
	glm::vec3 m_CameraUp;

};

