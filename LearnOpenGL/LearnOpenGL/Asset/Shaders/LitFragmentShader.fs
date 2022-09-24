#version 330 core

out vec4 FragColor;


in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
uniform vec3 viewPos;
uniform float cameraNear;
uniform float cameraFar;

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


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    //properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    ////directional light
    vec3 resultColor = vec3(0.0f, 0.0f, 0.0f);
    resultColor = CalcDirLight(dirLight, norm, viewDir);
    
    ////point lights
    //for(int i = 0; i < NR_POINT_LIGHTS; ++i)
    //{
    //    resultColor += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
    //}
    
    //Spot light
    resultColor += CalcSpotLight(spotLight, norm, fragPos, viewDir);
    

    //FragColor =  vec4(resultColor, 1.0f);

    float NDCDepth = gl_FragCoord.z * 2.0f - 1.0f; //From non-linear depth(0 ~ 1) in screen-space to NDC depth(-1 ~ 1)
    float viewNearFarDepth = (2.0f * cameraNear * cameraFar) / (cameraFar + cameraNear - NDCDepth * (cameraFar - cameraNear)); //From NDC depth(-1 ~ 1) to ViewSpace linear depth(0.1 ~ 100) near to far
    float colorDepth = viewNearFarDepth / cameraFar;    

    FragColor =  vec4(vec3(colorDepth), 1.0f);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //ambient
    vec3 ambientColor = vec3(texture(material.texture_diffuse1, texCoord)) * light.ambient;

    //diffuse
    vec3 lightDir = normalize(-light.direction);
    float diffuseStrength = max(dot(lightDir, normal), 0.0f);
    vec3 diffuseColor = vec3(texture(material.texture_diffuse1, texCoord)) * light.diffuse * diffuseStrength;

    //specular
    vec3 reflectLightDir = reflect(-lightDir, normal);
    float specStrength = pow(max(dot(viewDir, reflectLightDir), 0.0f), 32);
    vec3 specularColor = vec3(texture(material.texture_specular1, texCoord)) * light.specular * specStrength;

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
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    //ambient
    vec3 ambientColor = vec3(texture(material.texture_diffuse1, texCoord)) * light.ambient;

    //diffuse
    vec3 lightDirFromFragToLight = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(lightDirFromFragToLight, normal), 0.0f);
    vec3 diffuseColor = vec3(texture(material.texture_diffuse1, texCoord)) * light.diffuse * diffuseStrength * attenuation;

    //specular
    vec3 reflectLightDir = reflect(-lightDirFromFragToLight, normal);
    float specStrength = pow(max(dot(reflectLightDir, viewDir), 0.0f), 32);
    vec3 specularColor = vec3(texture(material.texture_specular1, texCoord)) * light.specular * specStrength * attenuation;

    //spot intensity
    float angleBetweenFragAndLight = dot(lightDirFromFragToLight, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float spotLightIntensity = clamp((angleBetweenFragAndLight - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    diffuseColor *= spotLightIntensity;
    specularColor *= spotLightIntensity;


    //final color
    return ambientColor + diffuseColor + specularColor;
}