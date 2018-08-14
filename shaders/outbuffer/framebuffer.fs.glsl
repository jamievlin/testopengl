#version 450

in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFrameBuffer;

const float sampleDimWidth = .01;
const int sampleDimSize = 8; 

void main()
{
    // postprocessing here

    /*
    vec4 sum = vec4(0);

    for (int i = -sampleDimSize; i <= sampleDimSize; i++)
    {
        for (int j = -sampleDimSize; j <= sampleDimSize; j++)
        {
            float u = Texcoord.x + (i/float(sampleDimSize) * sampleDimWidth);
            float v = Texcoord.y + (j/float(sampleDimSize) * sampleDimWidth);

            sum = sum + texture(texFrameBuffer, vec2(u, v));
        }
    }

    */

    // sum = sum / ((2 * sampleDimSize + 1) * (2 * sampleDimSize + 1));
    outColor = texture(texFrameBuffer, Texcoord);
}