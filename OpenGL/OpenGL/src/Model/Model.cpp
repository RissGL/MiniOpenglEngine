#include "Model.h"
#include "Base/ufbx.h"
#include <iostream>

Model::Model(const std::string& filepath, const std::string& targetMeshName)
{
    LoadModel(filepath, targetMeshName);
}

void Model::Draw(Shader& shader) const
{
    for (const auto& mesh : m_Meshes) {
        mesh->Draw(shader);
    }
}

void Model::LoadModel(const std::string& filepath, const std::string& targetMeshName)
{
    ufbx_load_opts opts = {};
    opts.target_axes = ufbx_axes_right_handed_y_up;
    opts.target_unit_meters = 1.0f;

    ufbx_error error;
    ufbx_scene* scene = ufbx_load_file(filepath.c_str(), &opts, &error);

    if (!scene) {
        std::cerr << "Failed to load FBX: " << error.description.data << std::endl;
        return;
    }

    // 获取 FBX 文件所在的文件夹路径，方便拼接贴图相对路径
    std::string directory = filepath.substr(0, filepath.find_last_of('/'));

    for (size_t i = 0; i < scene->nodes.count; i++) {
        ufbx_node* node = scene->nodes.data[i];
        if (!node->mesh) continue;

        std::string nodeName = node->name.data;
        std::cout << "[FBX] 发现模型部件: " << nodeName << std::endl;

        if (!targetMeshName.empty() && nodeName != targetMeshName) {
            continue; // 如果名字对不上，直接跳过不加载！
        }

        ufbx_mesh* mesh = node->mesh;

        std::string finalTexturePath = "";

        if (mesh->materials.count > 0) {

            ufbx_material* mat = mesh->materials.data[0];
            if (mat != nullptr) {
                if (mat->pbr.base_color.texture) {
                    finalTexturePath = mat->pbr.base_color.texture->relative_filename.data;
                }
                else if (mat->fbx.diffuse_color.texture) { // 兼容旧版 FBX 材质
                    finalTexturePath = mat->fbx.diffuse_color.texture->relative_filename.data;
                }

                if (!finalTexturePath.empty()) {
                    size_t lastSlash = finalTexturePath.find_last_of("/\\");
                    if (lastSlash != std::string::npos) {
                        finalTexturePath = finalTexturePath.substr(lastSlash + 1);
                    }

                    // 把纯文件名和我们自己的 FBX 文件夹路径拼起来
                    finalTexturePath = directory + "/" + finalTexturePath;
                    std::cout << "  -> 真正要加载的贴图: " << finalTexturePath << std::endl;
                }
            }
        }

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        uint32_t num_tri_indices = mesh->max_face_triangles * 3;
        std::vector<uint32_t> tri_indices(num_tri_indices);

        for (size_t face_idx = 0; face_idx < mesh->faces.count; face_idx++) {
            ufbx_face face = mesh->faces.data[face_idx];
            uint32_t num_tris = ufbx_triangulate_face(tri_indices.data(), tri_indices.size(), mesh, face);

            for (size_t j = 0; j < num_tris * 3; j++) {
                uint32_t index = tri_indices[j];
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
                    v.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
                }

                if (mesh->vertex_uv.exists) {
                    uint32_t uv_idx = mesh->vertex_uv.indices.data[index];
                    v.TexCoords.x = (float)mesh->vertex_uv.values.data[uv_idx].x;
                    v.TexCoords.y = (float)mesh->vertex_uv.values.data[uv_idx].y;
                }
                else {
                    v.TexCoords = glm::vec2(0.0f, 0.0f);
                }

                vertices.push_back(v);
                indices.push_back(vertices.size() - 1);
            }
        }

        m_Meshes.push_back(std::make_unique<Mesh>(vertices, indices, finalTexturePath));
    }

    ufbx_free_scene(scene);
}