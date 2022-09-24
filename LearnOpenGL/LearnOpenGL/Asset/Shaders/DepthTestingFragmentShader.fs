#version 330 core

out vec4 FragColor;

uniform float cameraNear;
uniform float cameraFar;

void main()
{
    float NDCDepth = gl_FragCoord.z * 2.0f - 1.0f; //From non-linear depth(0 ~ 1) in screen-space to NDC depth(-1 ~ 1)
    float viewNearFarDepth = (2.0f * cameraNear * cameraFar) / (cameraFar + cameraNear - NDCDepth * (cameraFar - cameraNear)); //From NDC depth(-1 ~ 1) to ViewSpace linear depth(0.1 ~ 100) near to far
    float colorDepth = viewNearFarDepth / cameraFar;    

    FragColor =  vec4(vec3(colorDepth), 1.0f);
}