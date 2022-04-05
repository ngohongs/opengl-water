#version 330 core
out vec4 FragColor;  
in vec2 UV;
uniform sampler2D tex;
uniform float texelSize;
uniform bool drop;
uniform vec2 dropPos;
uniform float deltaTime;

const float PI = 3.14159265359;

uniform bool abort;


uniform int size;

void main()
{
    if (abort) {
        FragColor = vec4(vec3(0.0), 1.0);
        return;
    }
    vec2 texCoord = UV;
    vec4 old_info = texture(tex, texCoord);

    vec2 offsetX = vec2(0.0, texelSize);
    vec2 offsetY = vec2(texelSize, 0.0);

    vec2 pdx = texCoord + offsetX;
    vec2 ndx = texCoord - offsetX;
    vec2 pdy = texCoord + offsetY;
    vec2 ndy = texCoord - offsetY;

    float vpdx = texture(tex, pdx).r;
    float vndx = texture(tex, ndx).r;
    float vpdy = texture(tex, pdy).r;
    float vndy = texture(tex, ndy).r;

    float nsum = vpdx + vndx + vpdy + vndy;
    float average =  (vpdx + vndx + vpdy + vndy) / 4.0;
    
    
   /*
    float old_u = old_info.r + heightWithDrop(texCoord);
    float old_v = old_info.g;
    float new_v = old_v + average - old_u;
    new_v *= 0.99;
    float new_u = old_u + new_v;
    */


    
    float h = texelSize;
    h = 1.0f / size;
    float old_u = old_info.r;// + height(texCoord);
    float old_v = old_info.g;


    float offset = average - old_u;
    float maxslope = 0.3;
    float maxoffset = maxslope * h;

    if (offset > maxoffset) 
        old_u = old_u + offset - maxoffset;
    if (offset < -maxoffset)
        old_u = old_u + offset + maxoffset;



  
    
    float c = 0.2f;
    float f = c * c * (nsum - 4 * old_u) / (h * h);
    float new_v = old_v + f * deltaTime;
    new_v = new_v - old_u;
    new_v = new_v;
    float new_u = old_u + new_v * deltaTime; 
    new_u = new_u;

//    float offset = average - new_u;
//    float maxslope = 10000;
//    float maxoffset = maxslope * h;
//
//    if (offset > maxoffset) 
//        new_u = new_u + offset - maxoffset;
//    if (offset < -maxoffset)
//        new_u = new_u + offset + maxoffset;

    vec4 new_info = vec4(new_u , new_v, 0, 1);



    FragColor = new_info;
}