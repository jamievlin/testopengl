#version 150

in vec3 position;
in vec2 texcoord;
in float intensity;

out float Intensity;
out vec2 Texcoord;

uniform mat4 transfMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform float scaleFactor;


void main()
{
    gl_Position = projMat * viewMat * vec4(position, 1.0);
    Intensity = intensity;
    Texcoord = texcoord;
}