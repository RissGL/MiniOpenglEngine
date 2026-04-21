#pragma once
#include "Input.h"
#include "Camera.h"
#include "MyTime.h"

enum class CameraType
{
    Editor,
    Game,
};

class  CameraController
{
public:
	 CameraController(Camera& camera,CameraType type=CameraType::Editor);

     // 每一帧更新键盘逻辑
     void OnUpdate(float deltaTime);

     // 处理鼠标拖拽平移和旋转逻辑
     void OnMouseUpdate();

     void SetCmeraSpeed(float newSpeed);

     float GetCameraMoveSpeed()const;

     Camera& GetCamera()const;

private:
	Camera& m_Camera;
    float m_MoveSpeed;
    float m_PanSpeed;

    float m_LastX;
    float m_LastY;
    bool m_FirstMouse;

    CameraType cameraType;
};
