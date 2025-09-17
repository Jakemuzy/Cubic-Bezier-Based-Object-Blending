#version 330 core
out vec4 FragColor;

uniform bool intersected;
uniform int currentDepth;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    if (intersected)
        FragColor = vec4(0.19f, 0.8f, 0.19f, 1.0f);
}
