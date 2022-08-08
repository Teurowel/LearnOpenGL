#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"

class Light
{
public:
    void Init(const glm::vec3& lightPosition,
              const glm::vec3& lightDirection,
              const glm::vec3& ambientColor,
              const glm::vec3& diffuseColor,
              const glm::vec3& specularColor,
              float constant, float linear, float quadratic);

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetDirection() const;
    
    const glm::vec3& GetAmbientColor() const;
    const glm::vec3& GetDiffuseColor() const;
    const glm::vec3& GetSpecularColor() const;

    float GetCosntant() const;
    float GetLinear() const;
    float GetQuadratic() const;
    
private :
    glm::vec3 lightPosition;
    glm::vec3 lightDirection;
    
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    float constant = 0.0f;
    float linear = 0.0f;
    float quadratic = 0.0f;
};

#endif