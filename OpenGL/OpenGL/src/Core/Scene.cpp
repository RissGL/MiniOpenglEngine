#include "Scene.h"

std::shared_ptr<GameObject> Scene::AddGameObject(const std::string& name)
{
    auto go = std::make_shared<GameObject>(name);
    m_GameObjects.push_back(go);
    return go;
}

void Scene::Awake()
{
    for (auto& go : m_GameObjects) {
        go->Awake();
    }
}

void Scene::Start()
{
    for (auto& go : m_GameObjects) {
        go->Start();
    }
}

void Scene::Update(float deltaTime)
{
    for (auto& go : m_GameObjects) {
        go->Update(deltaTime);
    }
}

void Scene::Draw(Shader& shader)
{
    for (auto& go : m_GameObjects) {
        go->Draw(shader);
    }
}
