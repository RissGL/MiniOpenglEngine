#include "MeshRenderer.h"

void MeshRenderer::Draw(Shader& shader)
{
    if (!model || !enabled) return;

    glm::mat4 modelMatrix = transform->GetModelMatrix();

    shader.SetUniformMat4f("u_Model", modelMatrix);

    model->Draw(shader);
}
