#version 330 core

out vec4 FragColor;


in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
uniform vec3 viewPos;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    float shininess;
};
uniform Material material;

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
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //attenuation
    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 3
uniform PointLight pointLights[NR_POINT_LIGHTS];


struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //attenuation
    float constant;
    float linear;
    float quadratic;

    float innerCutOff;
    float outerCutOff;
};
uniform SpotLight spotLight;


vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    //properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    ////directional light
    vec4 resultColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    resultColor = CalcDirLight(dirLight, norm, viewDir);
    
    ////point lights
    //for(int i = 0; i < NR_POINT_LIGHTS; ++i)
    //{
    //    resultColor += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
    //}
    
    //Spot light
    resultColor += CalcSpotLight(spotLight, norm, fragPos, viewDir);
    

    FragColor =  resultColor;
}


vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //ambient
    vec4 ambientColor = texture(material.texture_diffuse1, texCoord) * vec4(light.ambient, 1.0f);

    //diffuse
    vec3 lightDir = normalize(-light.direction);
    float diffuseStrength = max(dot(lightDir, normal), 0.0f);
    vec4 diffuseColor = texture(material.texture_diffuse1, texCoord) * vec4(light.diffuse, 1.0f) * diffuseStrength;

    //specular
    vec3 reflectLightDir = reflect(-lightDir, normal);
    float specStrength = pow(max(dot(viewDir, reflectLightDir), 0.0f), 32);
    vec4 specularColor = texture(material.texture_specular1, texCoord) * vec4(light.specular, 1.0f) * specStrength;

    //final color
    return ambientColor + diffuseColor + specularColor;
}
//
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    //attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0f / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));
//
//    //ambient
//    vec3 ambientColor = vec3(texture(material.diffuse, texCoord)) * light.ambient;
//
//    //diffuse
//    vec3 lightDir = normalize(light.position - fragPos);
//    float diffuseStrength = max(dot(lightDir, normal), 0.0f);
//    vec3 diffuseColor = vec3(texture(material.diffuse, texCoord)) * light.diffuse * diffuseStrength * attenuation;
//
//    //specular
//    vec3 reflectLightDir = reflect(-lightDir, normal);
//    float specStrength = pow(max(dot(reflectLightDir, viewDir), 0.0f), material.shininess);
//    vec3 specularColor = vec3(texture(material.specular, texCoord)) * light.specular * specStrength * attenuation;
//
//    //final color
//    return ambientColor + diffuseColor + specularColor;
//}
//
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    //ambient
    vec4 ambientColor = texture(material.texture_diffuse1, texCoord) * vec4(light.ambient, 1.0f);

    //diffuse
    vec3 lightDirFromFragToLight = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(lightDirFromFragToLight, normal), 0.0f);
    vec4 diffuseColor = texture(material.texture_diffuse1, texCoord) * vec4(light.diffuse, 1.0f) * diffuseStrength * attenuation;

    //specular
    vec3 reflectLightDir = reflect(-lightDirFromFragToLight, normal);
    float specStrength = pow(max(dot(reflectLightDir, viewDir), 0.0f), 32);
    vec4 specularColor = texture(material.texture_specular1, texCoord) * vec4(light.specular, 1.0f) * specStrength * attenuation;

    //spot intensity
    float angleBetweenFragAndLight = dot(lightDirFromFragToLight, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float spotLightIntensity = clamp((angleBetweenFragAndLight - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    diffuseColor *= spotLightIntensity;
    specularColor *= spotLightIntensity;


    //final color
    return ambientColor + diffuseColor + specularColor;
}