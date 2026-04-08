#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


class Input
{
public:

	static bool IsKeyPressed(int keycode);

	static bool IsMouseButtonPressed(int button);

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static void EndFrame();

	static float GetScrollY();

	static glm::vec2 GetMousePosition();

	static float GetMouseX();

	static float GetMouseY();

	static float s_ScrollY;
};