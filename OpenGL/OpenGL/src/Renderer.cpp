#include "Renderer.h"
#include <iostream>
#include "Debug/Debug.h"


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