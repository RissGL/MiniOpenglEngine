#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class MyWindow
{
public:
    MyWindow(int width, int height, const char* title);
    ~MyWindow();

    bool IsClosed() const;
    void OnUpdate();

    GLFWwindow* GetNativeWindow() const { return m_Window; }

    static float GetMouseOffsetX();
    static float GetMouseOffsetY();

	static int GetWidth() { return s_Width; }
	static int GetHeight() { return s_Height; }

private:
    GLFWwindow* m_Window;

    // 鼠标回调需要的静态变量
    static float s_LastX;
    static float s_LastY;
    static float s_OffsetX;
    static float s_OffsetY;
    static bool s_FirstMouse;

	static int s_Width;
	static int s_Height;

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
};