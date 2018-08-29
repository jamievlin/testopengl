#version 450

in vec3 position;
in vec3 normal;

out vec3 eyeNormal;
out vec3 worldNormal;
out vec3 lightDir; 
out vec3 worldPos;

uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 modelMat;

// inverse dual maps - i.e. transpose.
// for ortho transformation - just the transform itself.
// T^{-1}* for T: R4 -> R4

uniform mat4 viewMatInvDual;
uniform mat4 projMatInvDual;
uniform mat4 modelMatInvDual;

const vec4 vLightdir = vec4(-1, -1, 0, 0);

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0);
    worldPos = (modelMat * vec4(position, 1.0)).xyz; 

    lightDir = normalize(viewMat * vLightdir).xyz;

    worldNormal = normalize((modelMatInvDual * vec4(normal, 0.0)).xyz);
    eyeNormal = normalize((viewMatInvDual * modelMatInvDual * vec4(normal, 0.0)).xyz); 
}