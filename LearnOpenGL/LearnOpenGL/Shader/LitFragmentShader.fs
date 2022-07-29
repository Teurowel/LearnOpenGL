#version 330 core

out vec4 FragColor;


in vec2 texCoord;
uniform sampler2D texture0;
uniform sampler2D texture1;


in vec3 normal;
in vec3 fragPos;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.2f;
    vec3 ambientColor =  ambientStrength * lightColor;      
    
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 fragNormal = normalize(normal);
    float diffuseStrength = max(dot(fragNormal, lightDir), 0.0f);
    vec3 diffuseColor = diffuseStrength * lightColor;

    float specularStrength = 0.5f;
    float shininess = 32;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
    vec3 specularColor = specularStrength * spec * lightColor;    


    vec4 textureColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.5f);
    



    FragColor =  vec4(ambientColor + diffuseColor + specularColor, 1.0f) * textureColor; 
}