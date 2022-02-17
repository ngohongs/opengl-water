#version 330 core
out vec4 FragColor;  
in vec2 UV;
uniform sampler2D tex;
uniform float texelSize;
uniform bool drop;
uniform vec2 dropPos;

float heightWithDrop(vec2 pos)
{
    if (!drop)
        return 0.0;
    float len = length(pos - dropPos);
    return cos(max(0.0, 1-10*len))-1;

    float drop = max(0.0, 1.0 - length( pos - dropPos ) / 0.06);
	drop = 0.5 - cos(drop*3.1415926535899793)*0.5;
    return drop;
}

float height(vec2 pos)
{
	float drop = max(0.0, 1.0 - length( pos - dropPos ) / 0.06);
	drop = 0.5 - cos(drop*3.1415926535899793)*0.5;
    return drop;
}


void main()
{
    
    vec2 texCoord = UV;
    vec4 old_info = texture(tex, texCoord);

    vec2 offsetX = vec2(0.0, texelSize);
    vec2 offsetY = vec2(texelSize, 0.0);

    vec2 pdx = texCoord + offsetX;
    vec2 ndx = texCoord - offsetX;
    vec2 pdy = texCoord + offsetY;
    vec2 ndy = texCoord - offsetY;

    float vpdx = texture(tex, pdx).r + heightWithDrop(pdx);
    float vndx = texture(tex, ndx).r + heightWithDrop(ndx);
    float vpdy = texture(tex, pdy).r + heightWithDrop(pdy);
    float vndy = texture(tex, ndy).r + heightWithDrop(ndy);

    float average =  (vpdx + vndx + vpdy + vndy) / 4.0;
    
    float old_u = old_info.r + heightWithDrop(texCoord);
    float old_v = old_info.g;
    float new_v = old_v + average - old_u;
    new_v *= 0.99;
    float new_u = old_u + new_v;

    vec4 new_info = vec4(new_u, new_v, 0, 1);

    FragColor = new_info;
}