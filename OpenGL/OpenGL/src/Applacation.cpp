#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"

#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestMenu.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture3D.h"

#include "MyTime.h"
#include "Input.h"
#include "Window/MyWindow.h"

#include "Debug/Debug.h"

int main(void)
{
    MyWindow appWindow(1920, 1080, "OpenGL Debugging");

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(appWindow.GetNativeWindow(), true);
    ImGui::StyleColorsDark();

    glfwSetScrollCallback(appWindow.GetNativeWindow(), Input::ScrollCallback);

    Renderer renderer;
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Test Color");
    testMenu->RegisterTest<test::TestTexture2D>("Test Texture 2D");
    testMenu->RegisterTest<test::TestTexture3D>("Test Texture 3D");


    while (!appWindow.IsClosed())
    {
        MyTime::Update();
        renderer.Clear();
        ImGui_ImplGlfwGL3_NewFrame();

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
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        appWindow.OnUpdate();
    }

    if (currentTest != testMenu) delete currentTest;
    delete testMenu;

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    return 0;
}