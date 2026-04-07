#include "MyTime.h"
#include <GLFW/glfw3.h>

float MyTime::m_DeltaTime = 0.0f;
float MyTime::m_LastFrameTime = 0.0f;
float MyTime::m_Time = 0.0f;

float MyTime::GetDeltaTime()
{
	return m_DeltaTime;
}

float MyTime::GetTime()
{
	return m_Time;
}

void MyTime::Update()
{
	float currentTime = (float)glfwGetTime();
	m_DeltaTime = currentTime - m_LastFrameTime;
	m_LastFrameTime = currentTime;
	m_Time += m_DeltaTime;
}