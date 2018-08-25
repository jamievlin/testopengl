#version 450

in vec3 position;
in vec3 normal;

out vec4 Normal;

uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 modelMat;

// inverse dual maps - i.e. transpose.
// for ortho transformation - just the transform itself.
// T^{-1}* for T: R4 -> R4

uniform mat4 viewMatInvDual;
uniform mat4 projMatInvDual;
uniform mat4 modelMatInvDual;

void main()
{
    mat4 finPVMMat = projMat * viewMat * modelMat;
    mat4 finPVMMatInvDual = modelMatInvDual;

    gl_Position = finPVMMat * vec4(position, 1.0);
    Normal = finPVMMatInvDual * vec4(normal, 1.0); 
}