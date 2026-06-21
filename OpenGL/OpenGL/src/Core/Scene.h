#pragma once
#include "GameObject.h"
#include "Base/Shader.h"
#include <vector>
#include <memory>

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    std::shared_ptr<GameObject> AddGameObject(const std::string& name = "GameObject");

    void Awake();
    void Start();
    void Update(float deltaTime);
    void Draw(Shader& shader);

    const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const { return m_GameObjects; }
    std::vector<std::shared_ptr<GameObject>>& GetGameObjects() { return m_GameObjects; }

private:
    std::vector<std::shared_ptr<GameObject>> m_GameObjects;
};
