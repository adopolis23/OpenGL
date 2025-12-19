#version 430 core

in vec2 vUV;
out vec4 FragColor;

uniform int particleCount;
uniform float kernelRadius;

uniform vec2 worldMin; // (left, bottom)
uniform vec2 worldMax; // (right, top)

layout(std430, binding = 0) buffer Particles {
    vec2 positions[];
};

float kernel(float r, float d)
{
    float volume = (3.14159265358 * r * r * r * r * r * r * r * r / 4);

    float value = (r * r) - (d * d);
    value = (value > 0) ? value : 0;

    return ((value * value * value) / volume);
}

void main()
{
    vec2 worldPos = mix(worldMin, worldMax, vUV);
    float density = 0.0;
    
    // damps the effect of all density function, tunable.
    float damper_constant = 0.03;

    for (int i = 0; i < particleCount; ++i)
    {
        float d = distance(worldPos, positions[i]);
        if (d > kernelRadius) continue;
        density += (kernel(kernelRadius, d) * damper_constant);
    }

    density = (density > 1.0f ? 1.0f : density);
    FragColor = vec4(density, 0.0f, (1.0f-density), 1.0);
}
