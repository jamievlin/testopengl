#version 150

uniform vec3 triangleCol;
uniform float mixWeight;
uniform float _time;
uniform sampler2D texture1;
uniform sampler2D texture2;

in float Intensity;
in vec2 Texcoord;
out vec4 outColor;

void main()
{
    vec3 rawCol = Intensity * triangleCol;

    vec4 tex1 = texture(texture1, Texcoord); 
    vec4 tex2 = texture(texture2, Texcoord);

    // if (Texcoord.y >= 0.5)
    // {
    //     
    // }
    // else
    // {
    //     float newy = 1 - Texcoord.y;
    //     vec4 tex2_refl = 0.85 * texture(texture2, vec2(Texcoord.x + 0.02 * cos(Texcoord.y) * sin(5 * _time +  50 * Texcoord.y ), newy));
    //     outColor = vec4(rawCol, 1.0) * tex2_refl;
    // }

    outColor = mix(tex1, tex2, mixWeight) * vec4(rawCol, 1.0);
}