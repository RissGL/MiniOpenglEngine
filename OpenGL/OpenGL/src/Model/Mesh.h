#pragma once
#include "glm/glm.hpp"
#include <string>
#include "Base/Shader.h"
#include <memory>

#include "Base/VertexArray.h"
#include "Base/VertexBuffer.h"
#include "Base/IndexBuffer.h"

struct  Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	Mesh(const std::string& filepath);
	~Mesh();

	void Draw(Shader& shader)const;
private:
	std::unique_ptr<VertexArray> m_Vao;
	std::unique_ptr<VertexBuffer> m_Vbo;
	std::unique_ptr<IndexBuffer> m_Ibo;

	unsigned int m_IndexCount = 0;

	void LoadFBX(const std::string& filePath,std::vector<Vertex>& outVertices,std::vector<unsigned int>& outIndices);
};

