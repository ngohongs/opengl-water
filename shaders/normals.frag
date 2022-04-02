#version 330 core
out vec4 FragColor;

in vec2 fTexCoord;
in vec3 fNormal;

uniform mat4 projection;
uniform mat4 view;     
uniform mat4 model;  

uniform sampler2D tex;
uniform float texelSize;

uniform bool wave;

uniform bool inView;

void main()
{   
    vec2 px = vec2(texelSize, 0);
    vec2 nx = vec2(-texelSize, 0);
    vec2 py = vec2(0, texelSize);
    vec2 ny = vec2(0, -texelSize);


    float r = texture(tex, fTexCoord + px).r;
    float l = texture(tex, fTexCoord + nx).r;
    float t = texture(tex, fTexCoord + py).r;
    float b = texture(tex, fTexCoord + ny).r;
    float m = texture(tex, fTexCoord).r;

    vec3 normal;

    if (wave) {
        vec3 hor = vec3(2 * texelSize ,r - l,0);
        vec3 ver = vec3(0,t - b,2 * texelSize);
        normal = normalize(cross(ver, hor));
    }
    else {
        normal = normalize(fNormal);
    }
    if (inView) {
        normal = normalize(mat3(transpose(inverse(view * model))) * normal);
    }
    else {
        normal = normalize(mat3(transpose(inverse(model))) * normal);
    }
    FragColor = vec4(normal, 1.0);
}