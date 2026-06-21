#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Material/Texture.h"
#include "Model/Model.h"

class ResourceManager
{
public:
    static ResourceManager& Instance();

    std::shared_ptr<Texture> GetTexture(const std::string& path);
    std::shared_ptr<Model> GetModel(const std::string& path, const std::string& meshName = "");

    void Clear();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    std::unordered_map<std::string, std::shared_ptr<Model>> m_Models;

    std::string MakeModelKey(const std::string& path, const std::string& meshName);
};
