#version 450

in vec4 Normal;

out vec4 outColor;

void main()
{
    outColor = Normal;
}