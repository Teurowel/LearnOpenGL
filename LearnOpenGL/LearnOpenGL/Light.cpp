#include "Light.h"

void Light::Init(const glm::vec3& lightColor, const glm::vec3& lightPos)
{
    this->lightColor = lightColor;
    this->lightPos = lightPos;
}

const glm::vec3& Light::GetLightColor() const
{
    return lightColor;
}

const glm::vec3& Light::GetLightPos() const
{
    return lightPos;
}
