#version 330 core
out vec4 FragColor;

uniform int currentDepth;

void main()
{
    FragColor = vec4(1.0f / currentDepth, 0.0f, 0.0f, 1.0f);

} 
