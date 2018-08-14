#version 450

in vec2 position;
in vec2 texcoord;
out vec2 Texcoord;

void main()
{
    Texcoord = texcoord;
    gl_Position = vec4(position, 0, 1);
}