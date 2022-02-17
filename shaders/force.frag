#version 330 core
in vec2 texCoord;
uniform bool drop;
uniform vec2 dropPos;
uniform float texelSize;
uniform sampler2D tex;
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
    vec4 info = texture(tex, texCoord);

    float heightOffset = heightWithDrop(texCoord);

    vec2 offsetX = vec2(0.0, texelSize);
    vec2 offsetY = vec2(texelSize, 0.0);

    vec2 pdx = texCoord + offsetX;
    vec2 ndx = texCoord - offsetX;
    vec2 pdy = texCoord + offsetY;
    vec2 ndy = texCoord - offsetY;

    float vpdx = texture(tex, pdx).r + heightWithDrop(pdx);
    float vndx = texture(tex, ndx).r + heightWithDrop(ndx);
    float vpdy = texture(tex, pdy).r + heightWithDrop(pdy);
    float vndy = texture(tex, ndx).r + heightWithDrop(ndy);

    float average =  (vpdx + vndx + vpdy + vndy) / 4.0;
    
    float old_u = info.r + heightWithDrop(texCoord);
    float old_v = info.g;
    float new_v = old_v + average - old_u;
    new_v *= 0.99;
    float new_u = info.r + new_v;

    vec4 new_info = vec4(new_u, new_v, 0, 1);

    fragColor = new_info;
}
