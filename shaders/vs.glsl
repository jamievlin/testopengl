#version 150

in vec2 position;
in vec2 texcoord;

in float intensity;
out float Intensity;
out vec2 Texcoord;

void main()
{
    gl_Position = vec4(position.x, -position.y, 0.0, 1.0);
    Intensity = intensity;
    Texcoord = texcoord;
}