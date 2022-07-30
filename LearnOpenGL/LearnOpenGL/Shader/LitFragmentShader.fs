#version 330 core

out vec4 FragColor;


in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
uniform vec3 viewPos;


struct Material
{
    sampler2D diffuse;
    sampler2D specular;
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
    vec3 ambientColor =  vec3(texture(material.diffuse, texCoord)) * light.ambient;      
       
    //diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 fragNormal = normalize(normal);
    float diffuseStrength = max(dot(fragNormal, lightDir), 0.0f);
    vec3 diffuseColor = (diffuseStrength * vec3(texture(material.diffuse, texCoord))) * light.diffuse;

    //specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specularColor = (specularStrength * vec3(texture(material.specular, texCoord))) * light.specular;    
    
 
    FragColor =  vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}