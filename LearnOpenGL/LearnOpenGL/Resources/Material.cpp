#include "Material.h"

void Material::Init(const char* diffuseTextureKey, const char* specularTextureKey, float shininess)
{
    this->diffuseTextureKey = diffuseTextureKey;
    this->specularTextureKey = specularTextureKey;
    this->shininess = shininess;
}

void Material::Clear()
{
}

const char* Material::GetDiffuseTextureKey() const
{
    return diffuseTextureKey;
}

const char* Material::GetSpecularTextureKey() const
{
    return specularTextureKey;
}

float Material::GetShininess() const
{
    return shininess;
}
