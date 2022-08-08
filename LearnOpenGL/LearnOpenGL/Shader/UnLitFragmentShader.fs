#version 330 core

out vec4 FragColor;

uniform vec3 unLitColor;

void main()
{
    FragColor = vec4(unLitColor, 1.0f);
}