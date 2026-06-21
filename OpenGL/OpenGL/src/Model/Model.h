#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Base/Shader.h"

class Model
{
public:
    // 传文件路径给 Model
    Model(const std::string& filepath, const std::string& targetMeshName = "");

    // 画出整个模型
    void Draw(Shader& shader) const;

private:
    std::vector<std::unique_ptr<Mesh>> m_Meshes;

    void LoadModel(const std::string& filepath, const std::string& targetMeshName);
};