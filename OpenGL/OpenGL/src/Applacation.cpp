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

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1000, 1000, "OpenGL Debugging", NULL, NULL);
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
        float position[] = {
            -100.0f, -100.0f, 0.0f,0.0f,    //1.0f, 0.0f, 0.0f, 1.0f,
            -100.0f,  100.0f, 0.0f,1.0f,   //0.0f, 1.0f, 0.0f, 1.0f,
             100.0f,  100.0f, 1.0f,1.0f,    //0.0f, 0.0f, 1.0f, 1.0f,
             100.0f, -100.0f, 1.0f,0.0f    //0.0f, 0.0f, 1.0f, 1.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va;

        VertexBuffer vb(position, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f,1000.0f,0.0f,1000.0f,-1.0f,1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader("src/res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("src/res/texture/imgi_18_kea_ao.png");
        //Texture texture("src/res/texture/imgi_18_kea_ao.png");

        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        va.UnBind();
        shader.UnBind();
        vb.UnBind();
        ib.UnBind();

        ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

        Renderer renderer;

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        glm::vec3 translationA(0, 0, 0);

        glm::vec3 translationB(500, 200, 0);


        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            shader.Bind();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);     // Edit 1 float using a slider from 0.0f to 1.0f    
                ImGui::SliderFloat3("Translationb", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }


            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}