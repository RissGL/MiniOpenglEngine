#include "ResourceManager.h"

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& path)
{
    auto it = m_Textures.find(path);
    if (it != m_Textures.end()) {
        return it->second;
    }

    auto texture = std::make_shared<Texture>(path);
    m_Textures[path] = texture;
    return texture;
}

std::shared_ptr<Model> ResourceManager::GetModel(const std::string& path, const std::string& meshName)
{
    std::string key = MakeModelKey(path, meshName);
    auto it = m_Models.find(key);
    if (it != m_Models.end()) {
        return it->second;
    }

    auto model = std::make_shared<Model>(path, meshName);
    m_Models[key] = model;
    return model;
}

void ResourceManager::Clear()
{
    m_Textures.clear();
    m_Models.clear();
}

std::string ResourceManager::MakeModelKey(const std::string& path, const std::string& meshName)
{
    return path + "|" + meshName;
}
