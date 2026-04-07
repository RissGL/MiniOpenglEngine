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

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "OpenGL Debugging", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // 开启垂直同步，让动画更丝滑

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW 启动失败!" << std::endl;
    }

    // 获取版本信息不需要 GLCALL，因为这是查询
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    {
        ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

        Renderer renderer;

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

        (*testMenu).RegisterTest<test::TestClearColor>("Test Color");
		testMenu->RegisterTest<test::TestTexture2D>("Test Texture 2D");
		testMenu->RegisterTest<test::TestTexture3D>("Test Texture 3D");


        while (!glfwWindowShouldClose(window))
        {
			MyTime::Update();

            renderer.Clear(); // 移出 if，无论如何每一帧先清屏

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

            glfwSwapBuffers(window);
            glfwPollEvents();

        }
        if (currentTest != testMenu)
            delete currentTest;

        // 最后删掉菜单本身
        delete testMenu;

    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}