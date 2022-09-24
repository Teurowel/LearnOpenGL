#include "LitShader.h"
#include "../../Light.h"
#include "../../Game.h"

#include <memory>



void LitShader::Use()
{
    Shader::Use();

    //directional light set
	const std::shared_ptr<Camera> camera = game->GetCamera();
    const std::shared_ptr<Light> directionalLight = game->GetDirectionalLight();
    SetVec3("dirLight.direction", directionalLight->GetDirection());
    SetVec3("dirLight.ambient", directionalLight->GetAmbientColor());
    SetVec3("dirLight.diffuse", directionalLight->GetDiffuseColor());
    SetVec3("dirLight.specular", directionalLight->GetSpecularColor());

    //spot light set
    SetVec3("spotLight.position", camera->GetPosition());
    SetVec3("spotLight.direction", camera->GetCameraFront());

    const std::shared_ptr<Light> spotLight = game->GetSpotLight();
    SetVec3("spotLight.ambient", spotLight->GetAmbientColor());
    SetVec3("spotLight.diffuse", spotLight->GetDiffuseColor());
    SetVec3("spotLight.specular", spotLight->GetSpecularColor());
	
    SetFloat("spotLight.constant", spotLight->GetCosntant());
    SetFloat("spotLight.linear", spotLight->GetLinear());
    SetFloat("spotLight.quadratic", spotLight->GetQuadratic());
	
    SetFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
    SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
}
