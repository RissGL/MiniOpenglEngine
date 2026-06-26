#pragma once
#include "Core/GameObject.h"
#include "Core/MeshRenderer.h"
#include "Core/BoxCollider.h" 
#include "Model/Model.h"
#include <memory>
#include <string>
#include <vector>

class MazePrefabFactory
{
public:

    static std::shared_ptr<GameObject> CreateStraight(
        const std::string& name,
        std::shared_ptr<Model> model,
        std::vector<std::shared_ptr<GameObject>>& sceneObjects)
    {
        auto root = std::make_shared<GameObject>(name + "_Root");
        sceneObjects.push_back(root);

        auto visual = std::make_shared<GameObject>(name + "_Visual");
        visual->transform->SetParent(root->transform);

        visual->transform->localEulerAngles = glm::vec3(-90.0f, 0.0f, 0.0f);

        visual->transform->localScale = glm::vec3(1.0f);

        visual->AddComponent<MeshRenderer>(model);
        sceneObjects.push_back(visual);

        auto leftWall = std::make_shared<GameObject>(name + "_LWall");
        leftWall->transform->SetParent(root->transform);
        leftWall->transform->localPosition = glm::vec3(-2.0f, 0.0f, 0.0f);
        leftWall->AddComponent<BoxCollider>(glm::vec3(1.0f, 3.0f, 6.0f));
        sceneObjects.push_back(leftWall);

        auto rightWall = std::make_shared<GameObject>(name + "_RWall");
        rightWall->transform->SetParent(root->transform);
        rightWall->transform->localPosition = glm::vec3(2.0f, 0.0f, 0.0f);
        rightWall->AddComponent<BoxCollider>(glm::vec3(1.0f, 3.0f, 6.0f));
        sceneObjects.push_back(rightWall);

        return root; // 返回根节点，供地图生成器调用
    }


    static std::shared_ptr<GameObject> CreateCorner(
        const std::string& name,
        std::shared_ptr<Model> model,
        std::vector<std::shared_ptr<GameObject>>& sceneObjects)
    {
        auto root = std::make_shared<GameObject>(name + "_Root");
        sceneObjects.push_back(root);

        auto visual = std::make_shared<GameObject>(name + "_Visual");
        visual->transform->SetParent(root->transform);
        visual->transform->localEulerAngles = glm::vec3(-90.0f, 0.0f, 0.0f);
        visual->transform->localScale = glm::vec3(1.0f);
        visual->AddComponent<MeshRenderer>(model);
        sceneObjects.push_back(visual);

        auto leftWall = std::make_shared<GameObject>(name + "_OuterL");
        leftWall->transform->SetParent(root->transform);
        leftWall->transform->localPosition = glm::vec3(-2.0f, 0.0f, 0.0f);
        leftWall->AddComponent<BoxCollider>(glm::vec3(1.0f, 3.0f, 6.0f));
        sceneObjects.push_back(leftWall);

        auto frontWall = std::make_shared<GameObject>(name + "_OuterF");
        frontWall->transform->SetParent(root->transform);
        frontWall->transform->localPosition = glm::vec3(0.0f, 0.0f, -2.0f);
        frontWall->AddComponent<BoxCollider>(glm::vec3(6.0f, 3.0f, 1.0f));
        sceneObjects.push_back(frontWall);

        return root;
    }
};