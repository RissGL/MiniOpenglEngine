#pragma once
#include "Core/Component.h"
#include "Core/Transform.h"
#include "Core/GameObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

class BoxCollider:public Component
{
public:
    glm::vec3 size;   // 碰撞盒的长宽高
    glm::vec3 offset; // 碰撞盒相对于模型中心的偏移

    inline static std::vector<BoxCollider*> s_AllColliders;

    BoxCollider(glm::vec3 size = glm::vec3(1.0f), glm::vec3 offset = glm::vec3(0.0f))
        : size(size), offset(offset) {
    }
    void OnInspectorGUI() override;

    void Start() override
    {
        s_AllColliders.push_back(this);
    }

    ~BoxCollider()
    {
        // 销毁时自动从全局列表注销
        auto it = std::find(s_AllColliders.begin(), s_AllColliders.end(), this);
        if (it != s_AllColliders.end()) {
            s_AllColliders.erase(it);
        }
    }

    // 获取 AABB 的最小点和最大点 (左下后 和 右上前)
    glm::vec3 GetMin() const { return transform->GetWorldPosition() + offset - size * 0.5f; }
    glm::vec3 GetMax() const { return transform->GetWorldPosition() + offset + size * 0.5f; }

    bool IsOverlapping(BoxCollider* other) const;

};