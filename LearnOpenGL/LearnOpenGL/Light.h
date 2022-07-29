#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
    void Init(const glm::vec3& lightPos, const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor);
    
    const glm::vec3& GetLightPos() const;
    
    const glm::vec3& GetAmbientColor() const;
    const glm::vec3& GetDiffuseColor() const;
    const glm::vec3& GetSpecularColor() const;
    
private :
    glm::vec3 lightPos;
    
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    
};

#endif