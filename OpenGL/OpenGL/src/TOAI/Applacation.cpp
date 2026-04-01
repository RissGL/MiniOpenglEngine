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

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 640, "OpenGL Debugging", NULL, NULL);
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
            -0.8f, -0.8f, 0.0f,0.0f,    //1.0f, 0.0f, 0.0f, 1.0f,
            -0.8f,  0.8f, 0.0f,1.0f,   //0.0f, 1.0f, 0.0f, 1.0f,
             0.8f,  0.8f, 1.0f,1.0f,    //0.0f, 0.0f, 1.0f, 1.0f,
             0.8f, -0.8f, 1.0f,0.0f    //0.0f, 0.0f, 1.0f, 1.0f,
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

        glm::mat4 proj = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);

        Shader shader("src/res/shaders/Basic.shader");
        shader.Bind();

        //shader.SetUniform4f("u_Color",0.5f,0.5f,0.5f,0.5f);
        //shader.SetUniform1f("u_XOffset", 0.5f);
        shader.SetUniformMat4f("u_MVP",proj);

        Texture texture("src/res/texture/2000x2000bb-100 (4).png");
        //Texture texture("src/res/texture/imgi_18_kea_ao.png");

        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        va.UnBind();
        shader.UnBind();
        vb.UnBind();
        ib.UnBind();

        Renderer renderer;

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        while (!glfwWindowShouldClose(window))
        {
            float time = (float)glfwGetTime();
            float r = (sin(time) + 1.0f) / 2.0f;
            float g = (sin(time + 2.0f) + 1.0f) / 2.0f;
            float b = (sin(time + 4.0f) + 1.0f) / 2.0f;
            float xOffset = (sin(time) / 2.0f) * sin(2 * time + 3);

            renderer.Clear();
            renderer.Draw(va, ib, shader);

            // 设置 Uniform 需要 GLCALL
            //shader.SetUniform4f("u_Color", r, g, b, 1.0f);
            //shader.SetUniform1f("u_XOffset", xOffset);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        //GLCALL(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}