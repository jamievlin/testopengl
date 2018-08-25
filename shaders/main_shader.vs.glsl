#version 450

in vec3 position;
in vec3 normal;

out vec3 Normal;

uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 modelMat;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0);
    Normal = normal; 
}