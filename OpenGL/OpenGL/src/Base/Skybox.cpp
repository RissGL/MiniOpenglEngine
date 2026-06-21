#include "Skybox.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>

Skybox::Skybox()
{
    GenerateProceduralCubemap();
    SetupCubeMesh();
    m_Shader = std::make_unique<Shader>("src/res/shaders/Skybox.vert", "src/res/shaders/Skybox.frag");
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &m_CubemapID);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Skybox::GenerateProceduralCubemap()
{
    glGenTextures(1, &m_CubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapID);

    const int size = 64;
    std::vector<unsigned char> data(size * size * 3);

    // Procedural sky: gradient from bottom (dark) to top (light blue)
    // Each face gets a slightly different hue
    float faceTints[6][3] = {
        {1.0f, 1.1f, 1.2f}, // +X
        {0.9f, 0.9f, 1.1f}, // -X
        {0.8f, 0.9f, 1.0f}, // +Y (top, brighter)
        {0.3f, 0.3f, 0.4f}, // -Y (bottom, darker)
        {1.0f, 0.9f, 1.1f}, // +Z
        {0.9f, 1.0f, 0.9f}, // -Z
    };

    // Top color (sky blue) and bottom color (dark gray)
    float topColor[3]     = {0.45f, 0.65f, 0.95f};
    float bottomColor[3]  = {0.15f, 0.15f, 0.20f};
    float horizonColor[3] = {0.6f, 0.7f, 0.85f};

    for (int face = 0; face < 6; face++)
    {
        for (int y = 0; y < size; y++)
        {
            float t = (float)y / (float)(size - 1);

            // Create horizon effect
            float horizonWeight = 1.0f - abs(t - 0.3f) * 2.0f;
            horizonWeight = glm::clamp(horizonWeight, 0.0f, 1.0f);

            for (int x = 0; x < size; x++)
            {
                int idx = (y * size + x) * 3;

                // Mix between bottom, horizon, and top
                float topWeight = t;
                float botWeight = 1.0f - t;

                for (int c = 0; c < 3; c++)
                {
                    float base = bottomColor[c] * botWeight + topColor[c] * topWeight;
                    float withHorizon = glm::mix(base, horizonColor[c], horizonWeight * 0.5f);
                    data[idx + c] = (unsigned char)(withHorizon * faceTints[face][c] * 255.0f);
                }
            }
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB,
            size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::SetupCubeMesh()
{
    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void Skybox::Draw(const glm::mat4& view, const glm::mat4& projection) const
{
    glDepthFunc(GL_LEQUAL);
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_View", view);
    m_Shader->SetUniformMat4f("u_Projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapID);
    m_Shader->SetUniform1i("u_Skybox", 0);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}
