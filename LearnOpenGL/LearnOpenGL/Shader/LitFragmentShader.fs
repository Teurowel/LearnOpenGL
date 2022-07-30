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
    vec3 direction;
    float innerCutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;


void main()
{
    //ambient
    vec3 ambientColor =  vec3(texture(material.diffuse, texCoord)) * light.ambient;      
       
    //diffuse
    //Direction light
    //vec3 lightDir = normalize(-light.direction);

    //Point light, Spot light
    vec3 lightDirFromFragToLight = normalize(light.position - fragPos);
    vec3 fragNormal = normalize(normal);
    float diffuseStrength = max(dot(fragNormal, lightDirFromFragToLight), 0.0f);
    vec3 diffuseColor = (diffuseStrength * vec3(texture(material.diffuse, texCoord))) * light.diffuse;

    //specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDirFromFragToLight, fragNormal);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specularColor = (specularStrength * vec3(texture(material.specular, texCoord))) * light.specular;    
    

    //attenuation
    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + (light.linear * lightDistance) + (light.quadratic * (lightDistance * lightDistance)));     
    //FragColor =  vec4((ambientColor + diffuseColor + specularColor) * attenuation, 1.0f);
    ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;


    //SpotLight
    float angleBetweenFragAndLight = dot(lightDirFromFragToLight, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float spotLightIntensity = clamp((angleBetweenFragAndLight - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    diffuseColor *= spotLightIntensity;
    specularColor *= spotLightIntensity;



    FragColor =  vec4((ambientColor + diffuseColor + specularColor), 1.0f);
}