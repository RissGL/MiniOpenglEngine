#include "MyWindow.h"
#include "Input.h"

// 静态变量初始化
int MyWindow::s_Width = 1920;
int MyWindow::s_Height = 1080;
float MyWindow::s_LastX = 1920.0f / 2.0;
float MyWindow::s_LastY = 1080.0f / 2.0;
float MyWindow::s_OffsetX = 0.0f;
float MyWindow::s_OffsetY = 0.0f;
bool MyWindow::s_FirstMouse = true;

MyWindow::MyWindow(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        std::cout << "GLFW 初始化失败!" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!m_Window)
    {
        std::cout << "窗口创建失败!" << std::endl;
        glfwTerminate();
        return;
    }

	s_Height = height;
	s_Width = width;

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW 初始化失败!" << std::endl;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    //glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_Window, MouseCallback);

    glfwSetScrollCallback(m_Window, Input::ScrollCallback);

}

MyWindow::~MyWindow()
{
    glfwTerminate();
}

bool MyWindow::IsClosed() const
{
    return glfwWindowShouldClose(m_Window);
}

void MyWindow::OnUpdate()
{
    glfwSwapBuffers(m_Window);

    s_OffsetX = 0.0f;
    s_OffsetY = 0.0f;

    Input::EndFrame();

    glfwPollEvents();
}

void MyWindow::MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (s_FirstMouse)
    {
        s_LastX = xpos;
        s_LastY = ypos;
        s_FirstMouse = false;
    }

    s_OffsetX = xpos - s_LastX;
    s_OffsetY = s_LastY - ypos; 

    s_LastX = xpos;
    s_LastY = ypos;
}

float MyWindow::GetMouseOffsetX() { return s_OffsetX; }
float MyWindow::GetMouseOffsetY() { return s_OffsetY; }