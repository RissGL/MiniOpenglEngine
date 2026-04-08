#include "Input.h"

float Input::s_ScrollY = 0.0f;

float Input::GetScrollY()
{
	return s_ScrollY;
}

void Input::EndFrame()
{
	s_ScrollY = 0.0f;
}

bool Input::IsKeyPressed(int keycode)
{
	GLFWwindow* window = glfwGetCurrentContext();
	if (!window) return false; // 防止上下文丢失时的崩溃

	int state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
	GLFWwindow* window = glfwGetCurrentContext();
	if (!window) return false;

	int state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
	GLFWwindow* window = glfwGetCurrentContext();
	if (!window) return glm::vec2(0.0f, 0.0f);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return glm::vec2((float)xpos, (float)ypos);
}

float Input::GetMouseX()
{
	return GetMousePosition().x;
}

float Input::GetMouseY()
{
	return GetMousePosition().y;
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	s_ScrollY = (float)yoffset;
}