#ifndef UNLIT_SHADER_H
#define UNLIT_SHADER_H

#include "Shader.h"

class UnLitShader : public Shader
{
public :
    void Use() override;
    
};

#endif