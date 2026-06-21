#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "Texture.h"

class Texture;

struct Material
{
    std::shared_ptr<Texture> mapDiffuse;
    std::shared_ptr<Texture> specularMap;
    std::shared_ptr<Texture> emissionMap;

    float shininess = 32.0f;
};