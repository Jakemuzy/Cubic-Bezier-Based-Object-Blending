#version 330 core
out vec4 FragColor;

uniform int currentDepth;

void main()
{
    float depthFactor = clamp(float(currentDepth) / 5.0, 0.0, 1.0);

    vec3 color = vec3(
        0.5 + 0.5 * sin(depthFactor * 3.1415),  // red
        0.5 + 0.5 * cos(depthFactor * 3.1415),  // green
        1.0 - depthFactor                        // blue
    );

    float alpha = 0.3 + 0.7 * (1.0 - depthFactor);

    FragColor = vec4(color, alpha);
}
