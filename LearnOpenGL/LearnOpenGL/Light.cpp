#include "Light.h"

void Light::Init(const glm::vec3& lightPosition,
    const glm::vec3& lightDirection,
    const glm::vec3& ambientColor,
    const glm::vec3& diffuseColor,
    const glm::vec3& specularColor,
    float constant, float linear, float quadratic)
{
    this->lightPosition = lightPosition;
    this->lightDirection = lightDirection;

    this->ambientColor = ambientColor;
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;

    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

const glm::vec3& Light::GetLightPosition() const
{
    return lightPosition;
}

const glm::vec3& Light::GetLightDirection() const
{
    return lightDirection;
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

float Light::GetCosntant() const
{
    return constant;
}

float Light::GetLinear() const
{
    return linear;
}

float Light::GetQuadratic() const
{
    return quadratic;
}


