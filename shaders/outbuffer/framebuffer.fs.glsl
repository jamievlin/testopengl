#version 450

in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFrameBuffer;

void main()
{
    outColor =  vec4(1, 1, 1, 1) - texture(texFrameBuffer, Texcoord);
}