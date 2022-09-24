#include "StencilTestingShader.h"

void StencilTestingShader::Use()
{
    Shader::Use();

    SetVec3("outlineColor", glm::vec3(1.0f, 0.0f, 0.0f));
}
