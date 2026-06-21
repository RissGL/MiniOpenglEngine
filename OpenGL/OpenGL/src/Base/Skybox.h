#pragma once
#include <GL/glew.h>
#include <memory>
#include <array>
#include "Shader.h"

class Skybox
{
public:
    Skybox();
    ~Skybox();

    void Draw(const glm::mat4& view, const glm::mat4& projection) const;

private:
    unsigned int m_CubemapID = 0;
    unsigned int m_VAO = 0, m_VBO = 0;
    std::unique_ptr<Shader> m_Shader;

    void GenerateProceduralCubemap();
    void SetupCubeMesh();
};
