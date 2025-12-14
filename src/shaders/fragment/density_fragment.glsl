#version 330 core

in vec2 vUV;
uniform sampler2D uDensity;

out vec4 FragColor;

void main()
{
    float d = texture(uDensity, vUV).r;
    FragColor = vec4(d, d, d, 1.0);
}