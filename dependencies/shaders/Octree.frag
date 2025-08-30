#version 330 core
out vec4 FragColor;

uniform int currentDepth = 1;

void main()
{
    FragColor = vec4(currentDepth * 32.0f, 1.0f, 1.0f, 1.0f);
} 
