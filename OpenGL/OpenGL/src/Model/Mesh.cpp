#include "Mesh.h"
#include "Base/VertexBufferLayout.h"
#include "Base/ufbx.h"
#include <iostream>

Mesh::Mesh(const std::string& filepath)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	LoadFBX(filepath, vertices, indices);
	m_IndexCount = indices.size();

	if (vertices.empty() || indices.empty()) {
		std::cout << "Warning: 网格没有顶点或索引" << std::endl;
		return;
	}

	m_Vao = std::make_unique<VertexArray>();

	m_Vbo = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
	m_Ibo = std::make_unique<IndexBuffer>(&indices[0], indices.size());

	VertexBufferLayout layout;
	layout.Push<float>(3);//位置
	layout.Push<float>(3);//法线
	layout.Push<float>(2);//纹理坐标
	m_Vao->AddBuffer(*m_Vbo,layout);
}

Mesh::~Mesh()
{
}


void Mesh::Draw(Shader& shader) const
{
	if (m_IndexCount == 0)
		return;

	shader.Bind();
	m_Vao->Bind();
	m_Ibo->Bind();

	GLCALL(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr));
}

void Mesh::LoadFBX(const std::string& filePath, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices)
{
	ufbx_load_opts opts = {};
	opts.target_axes = ufbx_axes_right_handed_y_up; // 自动转为 OpenGL 右手坐标系！
	opts.target_unit_meters = 1.0f;                 // 统一缩放单位

	ufbx_error error;
	ufbx_scene* scene = ufbx_load_file(filePath.c_str(), &opts, &error);

	if (!scene) {
		std::cerr << "加载FBX失败: " << error.description.data << std::endl;
		return;
	}

    for (size_t i = 0; i < scene->meshes.count; i++) {
        ufbx_mesh* mesh = scene->meshes.data[i];

        uint32_t num_tri_indices = mesh->max_face_triangles * 3;
        std::vector<uint32_t> tri_indices(num_tri_indices);

        for (size_t face_idx = 0; face_idx < mesh->faces.count; face_idx++) {
            ufbx_face face = mesh->faces.data[face_idx];
            // 把多边形转成三角形
            uint32_t num_tris = ufbx_triangulate_face(tri_indices.data(), tri_indices.size(), mesh, face);

            for (size_t j = 0; j < num_tris * 3; j++) {
                uint32_t index = tri_indices[j]; // 重点：这是多边形面的“面顶点”全局索引

                Vertex v;

                uint32_t pos_idx = mesh->vertex_position.indices.data[index];
                v.Position.x = (float)mesh->vertex_position.values.data[pos_idx].x;
                v.Position.y = (float)mesh->vertex_position.values.data[pos_idx].y;
                v.Position.z = (float)mesh->vertex_position.values.data[pos_idx].z;

                if (mesh->vertex_normal.exists) {
                    uint32_t norm_idx = mesh->vertex_normal.indices.data[index];
                    v.Normal.x = (float)mesh->vertex_normal.values.data[norm_idx].x;
                    v.Normal.y = (float)mesh->vertex_normal.values.data[norm_idx].y;
                    v.Normal.z = (float)mesh->vertex_normal.values.data[norm_idx].z;
                }
                else {
                    v.Normal = glm::vec3(0.0f, 1.0f, 0.0f); // 默认法线朝上
                }

                if (mesh->vertex_uv.exists) {
                    uint32_t uv_idx = mesh->vertex_uv.indices.data[index];
                    v.TexCoords.x = (float)mesh->vertex_uv.values.data[uv_idx].x;
                    v.TexCoords.y = (float)mesh->vertex_uv.values.data[uv_idx].y;
                }
                else {
                    v.TexCoords = glm::vec2(0.0f, 0.0f);
                }

                outVertices.push_back(v);
                outIndices.push_back(outVertices.size() - 1);
            }
        }
    }

    ufbx_free_scene(scene);
}

