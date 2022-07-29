#version 330 core

out vec4 FragColor;


in vec2 texCoord;
uniform sampler2D texture0;
uniform sampler2D texture1;


in vec3 normal;
in vec3 fragPos;
uniform vec3 viewPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    //ambient
    vec3 ambientColor =  material.ambient * light.ambient;      
       
    //diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 fragNormal = normalize(normal);
    float diffuseStrength = max(dot(fragNormal, lightDir), 0.0f);
    vec3 diffuseColor = (diffuseStrength * material.diffuse) * light.diffuse;

    //specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specularColor = (specularStrength * material.specular) * light.specular;    


    vec4 textureColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.5f);
    

    //FragColor =  vec4(ambientColor + diffuseColor + specularColor, 1.0f) * textureColor; 
    FragColor =  vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}