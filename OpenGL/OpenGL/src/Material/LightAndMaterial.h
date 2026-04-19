#pragma once
#include "glm/glm.hpp"
#include "Texture.h"

class Texture;

struct Material
{
    Texture* mapDiffuse;
    Texture* specularMap;
    Texture* emissionMap;

    float shininess;
};

struct Light
{
    glm::vec3 lightDirection;
    glm::vec3 baseColor; // 光源本色
    float intensity;     // 光源强度

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 attenuation;

    void Update()
    {
        ambient = baseColor * 0.23f;               
        diffuse = baseColor * intensity;          
        specular = baseColor * intensity;         
    }
};