#include "Mesh.h"
#include "Base/VertexBufferLayout.h"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::string& texturePath)
{
    m_IndexCount = indices.size();

    if (vertices.empty() || indices.empty()) {
        std::cout << "警告:没有顶点或索引" << std::endl;
        return;
    }

    if (!texturePath.empty()) {
        m_Texture = std::make_shared<Texture>(texturePath);
    }

    m_Vao = std::make_unique<VertexArray>();
    m_Vbo = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
    m_Ibo = std::make_unique<IndexBuffer>(&indices[0], indices.size());

    VertexBufferLayout layout;
    layout.Push<float>(3); // Position
    layout.Push<float>(3); // Normal
    layout.Push<float>(2); // TexCoords
    m_Vao->AddBuffer(*m_Vbo, layout);
}

Mesh::~Mesh() {}

void Mesh::Draw(Shader& shader) const
{
    if (!m_Vao || !m_Ibo || m_IndexCount == 0) return;

    if (m_Texture) {
        m_Texture->Bind(0);
    }

    shader.Bind();
    m_Vao->Bind();
    m_Ibo->Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr));
}