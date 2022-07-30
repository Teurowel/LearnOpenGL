#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
public:
    void Init(const char* diffuseTextureKey, const char* specularTextureKey, float shininess);
    void Clear();

    const char* GetDiffuseTextureKey() const;
    const char* GetSpecularTextureKey() const;
    float GetShininess() const;
    
private :
    const char* diffuseTextureKey;
    const char* specularTextureKey;
    float shininess = 0.0f;    
};

#endif