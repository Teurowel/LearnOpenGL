#include "DepthTestingShader.h"

#include "../../Game.h"
#include "../../Camera.h"

void DepthTestingShader::Use()
{
    Shader::Use();

    const std::shared_ptr<Camera> camera = game->GetCamera();
    SetFloat("cameraNear", camera->GetCameraNear());
    SetFloat("cameraFar", camera->GetCameraFar());
}
