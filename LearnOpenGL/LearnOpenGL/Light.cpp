#include "Light.h"

#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"

void Light::Init(const glm::vec3& lightPos, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor)
{
    this->lightPos = lightPos;

    this->ambientColor = ambientColor;
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
}

const glm::vec3& Light::GetLightPos() const
{
    return lightPos;
}

const glm::vec3& Light::GetAmbientColor() const
{
    return ambientColor;
}

const glm::vec3& Light::GetDiffuseColor() const
{
    return diffuseColor;
}

const glm::vec3& Light::GetSpecularColor() const
{
    return specularColor;
}


