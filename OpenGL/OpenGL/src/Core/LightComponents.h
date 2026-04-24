#pragma once
#include "Core/Component.h"
#include "Core/Transform.h"
#include "Light/PointLight.h"
#include "Light/DirLight.h"
#include "Light/SpotLight.h"
#include <memory>

class PointLightComponent : public Component
{
public:
    std::shared_ptr<PointLight> lightData;

    PointLightComponent(std::shared_ptr<PointLight> light) : lightData(light) {}

    void Update(float deltaTime) override
    {
        if (lightData && transform) {
            lightData->position = transform->localPosition;
        }
    }

    void OnInspectorGUI() override;

};

class DirLightComponent : public Component
{
public:
    std::shared_ptr<DirLight> lightData;

    DirLightComponent(std::shared_ptr<DirLight> light) : lightData(light) {}

    void Update(float deltaTime) override
    {
        if (lightData && transform) {
            glm::vec3 euler = transform->localEulerAngles;
            glm::vec3 dir;
            dir.x = cos(glm::radians(euler.y)) * cos(glm::radians(euler.x));
            dir.y = sin(glm::radians(euler.x));
            dir.z = sin(glm::radians(euler.y)) * cos(glm::radians(euler.x));
            lightData->lightDirection = glm::normalize(dir);
        }
    }

    void OnInspectorGUI() override;

};

class SpotLightComponent : public Component
{
public:
    std::shared_ptr<SpotLight> lightData;

    SpotLightComponent(std::shared_ptr<SpotLight> light) : lightData(light) {}

    void Update(float deltaTime) override
    {
        if (lightData && transform) {
            // Î»ÖÃ¸úËæ
            lightData->position = transform->localPosition;

            // ³¯Ïò¸úËæ
            glm::vec3 euler = transform->localEulerAngles;
            glm::vec3 dir;
            dir.x = cos(glm::radians(euler.y)) * cos(glm::radians(euler.x));
            dir.y = sin(glm::radians(euler.x));
            dir.z = sin(glm::radians(euler.y)) * cos(glm::radians(euler.x));
            lightData->direction = glm::normalize(dir);
        }
    }

    void OnInspectorGUI() override;

};