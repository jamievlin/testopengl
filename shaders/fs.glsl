#version 150

uniform vec3 triangleCol;

in float Intensity;
out vec4 outColor;

void main()
{
    vec3 rawCol = vec3(1, 1, 1) - Intensity * triangleCol;
    outColor = vec4(rawCol, 1);
}