#version 330 core
in vec2 texCoord;
uniform vec2 dropPos;
uniform sampler2D tex;
uniform bool drop;
out vec4 fragColor;

float heightWithDrop(vec2 pos)
{
    if (!drop)
        return 0.0;
    float len = length(pos - dropPos);
    return -cos(max(0.0, 1-10*len))+1;
}

void main()
{
    float offset = heightWithDrop(texCoord) * 0.6;
    fragColor = vec4(texture(tex, texCoord).rgb + vec3(offset, 0, 0), 1);
}