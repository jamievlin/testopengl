#version 450

in vec3 eyeNormal;
in vec3 worldNormal;
in vec3 lightDir; 
in vec3 worldPos;

out vec4 outColor;

const vec3 light = vec3(1, 1, 1);
const vec3 diffuse = vec3(0.65, 0, 0);

const float bias = 0.04; 
const float scale = .65; 
const float power = 4; 

uniform vec3 cameraPos;
uniform samplerCube cubeMap;


void main()
{
    vec3 rawColor = diffuse * light * clamp(dot(eyeNormal, -lightDir), 0, 1);
    vec3 incidence = normalize(worldPos - cameraPos); 
    rawColor += diffuse * vec3(.506, .514, 0.635);
    rawColor = clamp(rawColor, 0, 1);


    vec3 reflDir = normalize(reflect(incidence, worldNormal));
    vec3 reflCol = texture(cubeMap, reflDir).rgb; 



    // fresnel
    // from http://kylehalladay.com/blog/tutorial/2014/02/18/Fresnel-Shaders-From-The-Ground-Up.html

    float reflectance = clamp(bias + (scale * pow(1 + dot(worldNormal, incidence), power)), 0, 1);
    
    outColor = vec4(mix(rawColor, reflCol, reflectance), 1);

}