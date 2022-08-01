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


//struct Light
//{
//    vec3 position;
//    vec3 direction;
//    float innerCutOff;
//    float outerCutOff;
//    
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//
//    float constant;
//    float linear;
//    float quadratic;
//};
//uniform Light light;

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight
{
    vec3 position
       
    //attenuation
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    //properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    //directional light
    vec3 resultColor = CalcDirLight(dirLight, norm, viewDir);
    
    //point lights
    for(int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
        resultColor += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
    }
    
    //SpotLight
//    float angleBetweenFragAndLight = dot(lightDirFromFragToLight, normalize(-light.direction));
//    float epsilon = light.innerCutOff - light.outerCutOff;
//    float spotLightIntensity = clamp((angleBetweenFragAndLight - light.outerCutOff) / epsilon, 0.0f, 1.0f);
//
//    diffuseColor *= spotLightIntensity;
//    specularColor *= spotLightIntensity;



    FragColor =  vec4(resultColor, 1.0f);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //ambient
    vec3 ambientColor = vec3(texture(material.diffuse, texCoord)) * light.ambient;

    //diffuse
    vec3 lightDir = normalize(-light.direction);
    float diffuseStrength = max(dot(lightDir, normal), 0.0f);
    vec3 diffuseColor = vec3(texture(material.diffuse, texCoord)) * light.diffuse * diffuseStrength;

    //specular
    vec3 reflectLightDir = reflect(-lightDir, normal);
    float specStrength = pow(max(dot(viewDir, reflectLightDir), 0.0f), material.shininess);
    vec3 specularColor = vec3(texture(material.specular, texCoord)) * light.specular * specStrength;

    //final color
    return ambientColor + diffuseColor + specularColor;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //attenuation
    float distance = length(light.position - fragPos);
    flaot attenuation = 1.0f / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    //ambient
    vec3 ambientColor = vec3(texture(material.diffuse, texCoord)) * light.ambient;

    //diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(lightDir, normal), 0.0f);
    vec3 diffuseColor = vec3(texture(material.diffuse, texCoord)) * light.diffuse * diffuseStrength * attenuation;

    //specular
    vec3 reflectLightDir = reflect(-lightDir, normal);
    vec3 specStrength = pow(max(dot(reflectLightDir, viewDir), 0.0f), material.shininess);
    vec3 specularColor = vec3(texture(material.specular, texCoord)) * light.specular * specStrength * attenuation;

    //final color
    return ambientColor + diffuseColor + specularColor;
}