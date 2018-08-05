#version 150

uniform vec3 triangleCol;

in float Intensity;
in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D tex;

void main()
{
    vec3 rawCol = vec3(1, 1, 1) - Intensity * triangleCol;
    outColor = texture(tex, Texcoord) * vec4(rawCol, 1);

}