#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0f);
    
    texCoord = aTexCoord;
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    normal = normalMatrix * aNormal;
    fragPos = vec3(worldPos);
    
    gl_Position = projection * view * worldPos;
}