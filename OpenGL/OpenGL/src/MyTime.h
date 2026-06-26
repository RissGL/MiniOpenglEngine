#pragma once

class MyTime
{
public:
	static void Update();
	static float GetDeltaTime();
	static float GetTime();

private:
	static float m_DeltaTime;
	static float m_LastFrameTime;
	static float m_Time;
};
