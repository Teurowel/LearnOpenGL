#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
    void Init(const glm::vec3& lightColor, const glm::vec3& lightPos);


    const glm::vec3& GetLightColor() const;
    const glm::vec3& GetLightPos() const;
private :
    glm::vec3 lightColor;
    glm::vec3 lightPos;
};

#endif