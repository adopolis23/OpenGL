#version 330 core

in vec2 vUV;
uniform sampler2D uDensity;

out vec4 FragColor;

void main()
{
    // damps the effect of all density function, tunable.
    float damper_constant = 0.02;

    float d = texture(uDensity, vUV).r;
    d *= damper_constant;
    d = clamp(d, 0.0, 1.0);
    FragColor = vec4(d, 0.0, d, 1.0);
}