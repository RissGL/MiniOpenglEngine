#pragma once
#include "Component.h"
#include "Model/Model.h"
#include "Transform.h" // ĞèÒªÄÃµ½¾ØÕó
#include <memory>

class MeshRenderer : public Component
{
public:
    std::shared_ptr<Model> model;

    MeshRenderer(std::shared_ptr<Model> m = nullptr) : model(m) {}
    ~MeshRenderer()override = default;

    void Draw(Shader& shader) override;

};