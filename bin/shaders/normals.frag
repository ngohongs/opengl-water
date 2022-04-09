#version 420 core
in vec2 fTexCoord;
in vec3 fNormal;

uniform mat4 projection;
uniform mat4 view;     
uniform mat4 model;  
uniform float texelSize;
uniform bool wave;
layout(binding=0) uniform sampler2D heightField;

out vec4 FragColor;

void main()
{   
    vec2 px = vec2(texelSize, 0);
    vec2 nx = vec2(-texelSize, 0);
    vec2 py = vec2(0, texelSize);
    vec2 ny = vec2(0, -texelSize);


    float r = texture(heightField, fTexCoord + px).r;
    float l = texture(heightField, fTexCoord + nx).r;
    float t = texture(heightField, fTexCoord + py).r;
    float b = texture(heightField, fTexCoord + ny).r;
    float m = texture(heightField, fTexCoord).r;

    vec3 normal;

    if (wave) {
        vec3 hor = vec3(2 * texelSize ,r - l,0);
        vec3 ver = vec3(0,t - b,2 * texelSize);
        normal = normalize(cross(ver, hor));
    }
    else {
        normal = normalize(fNormal);
    }
 
 
    normal = normalize(mat3(transpose(inverse(model))) * normal);

    FragColor = vec4(normal, 1.0);
}