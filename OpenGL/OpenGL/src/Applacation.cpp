#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "Base/Renderer.h"
#include "Base/VertexBuffer.h"
#include "Base/IndexBuffer.h"
#include "Base/VertexArray.h"
#include "Base/Shader.h"
#include "Base/VertexBufferLayout.h"

#include "Material/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestMenu.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture3D.h"

#include "MyTime.h"
#include "Input.h"
#include "Window/MyWindow.h"

#include "Debug/Debug.h"

#include "tests/TestIllumination.h"

int main(void)
{
    MyWindow appWindow(2560, 1600, "OpenGL Debugging");

    // 现代 ImGui 初始化 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //  开启 Docking 和 多窗口魔法！
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 允许键盘控制
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // 开启窗口停靠
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // 开启多视图 

    //  加载中文字体 (替换成你电脑里的字体路径)
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 20.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

    ImGui::StyleColorsDark();

    // 如果开启了多窗口，让非活跃窗口稍微透明一点，看起来更高级
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // 初始化新的后端
    ImGui_ImplGlfw_InitForOpenGL(appWindow.GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glfwSetScrollCallback(appWindow.GetNativeWindow(), Input::ScrollCallback);

    Renderer renderer;
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Test Color");
    //testMenu->RegisterTest<test::TestTexture2D>("Test Texture 2D");
    testMenu->RegisterTest<test::TestTexture3D>("Test Texture 3D");
    testMenu->RegisterTest<test::TestIllumination>("Test Illumination");

    while (!appWindow.IsClosed())
    {
        MyTime::Update();
        renderer.Clear();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        if (currentTest)
        {
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();

            ImGui::Begin("Test Framework");
            if (currentTest != testMenu && ImGui::Button("<- Back"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        appWindow.OnUpdate();
    }

    if (currentTest != testMenu) delete currentTest;
    delete testMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImGui::DestroyContext();
    return 0;
}