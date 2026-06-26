#pragma once
#include <string>
#include <vector>
#include <memory>
#include "glm/glm.hpp"

#include "Base/VertexArray.h"
#include "Base/VertexBuffer.h"
#include "Base/IndexBuffer.h"
#include "Base/Shader.h"

#include "Material/Texture.h" 

// 顶点结构体保留在这里
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::string& texturePath = "");
    ~Mesh();

    void Draw(Shader& shader) const;

private:
    std::unique_ptr<VertexArray> m_Vao;
    std::unique_ptr<VertexBuffer> m_Vbo;
    std::unique_ptr<IndexBuffer> m_Ibo;
    std::shared_ptr<Texture> m_Texture;

    unsigned int m_IndexCount = 0;
};

