#version 330 core
in vec2 fTexCoord;

uniform float deltaTime;

uniform float texelSize;

uniform bool drop;
uniform vec2 dropPos;
uniform float radius;
uniform float amplitude;

uniform bool abort;

uniform float waveSpeed;
uniform float waveDamping;
uniform float waveSlope;

uniform sampler2D heightField;

out vec4 FragColor;  

const float PI = 3.14159265359;

float AddDrop(vec2 pos)
{
    if (!drop)
        return 0.0;
    float r = max(radius, 0.001f)  * texelSize;
	float val = max(0.0, 1.0 - length(dropPos - pos) / r);
    val = 0.5 - cos(val * PI) * 0.5;
    return amplitude * val;
}

void main()
{
    if (abort) {
        FragColor = vec4(vec3(0.0), 1.0);
        return;
    }

    vec4 old_info = texture(heightField, fTexCoord);

    vec2 offsetX = vec2(0.0, texelSize);
    vec2 offsetY = vec2(texelSize, 0.0);

    vec2 pdx = fTexCoord + offsetX;
    vec2 ndx = fTexCoord - offsetX;
    vec2 pdy = fTexCoord + offsetY;
    vec2 ndy = fTexCoord - offsetY;

    float vpdx = texture(heightField, pdx).r + AddDrop(pdx);
    float vndx = texture(heightField, ndx).r + AddDrop(ndx);
    float vpdy = texture(heightField, pdy).r + AddDrop(pdy);
    float vndy = texture(heightField, ndy).r + AddDrop(ndy);

    float nsum = vpdx + vndx + vpdy + vndy;
    float average =  (vpdx + vndx + vpdy + vndy) / 4.0;

    
    float h = texelSize;
    float old_u = old_info.r + AddDrop(fTexCoord);
    float old_v = old_info.g;


    float offset = average - old_u;
    float maxslope = waveSlope;
    float maxoffset = maxslope * h;

    if (offset > maxoffset) 
        old_u = old_u + offset - maxoffset;
    if (offset < -maxoffset)
        old_u = old_u + offset + maxoffset;
    
    float c = max(waveSpeed, 0.001f);
    float f = c * c * (nsum - 4 * old_u) / (h * h);
    float new_v = old_v + f * deltaTime;
    new_v = new_v - old_u;
    new_v = max((1 - waveDamping), 0.001f) * new_v;
    float new_u = old_u + new_v * deltaTime; 
    new_u = new_u;

    vec4 new_info = vec4(new_u , new_v, 0, 1);

    FragColor = new_info;
}