#version 150

in vec2 position;
in vec2 texcoord;
in float intensity;

out float Intensity;
out vec2 Texcoord;

uniform mat4 transfMat;
uniform mat4 viewMat;
uniform mat4 projMat;


void main()
{
    gl_Position = projMat * viewMat * transfMat * vec4(position.x, -position.y, 0.0, 1.0);
    Intensity = intensity;
    Texcoord = texcoord;
}