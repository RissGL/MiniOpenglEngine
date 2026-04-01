#include "Renderer.h"
#include <iostream>

void GlClearError()
{
    // glGetError 会返回错误标志，调用后会被重置为 GL_NO_ERROR，所以需要循环清空
    while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear()const
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)const 
{
    shader.Bind();

    va.Bind();
    ib.Bind();


    // 核心绘制调用
    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}