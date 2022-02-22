#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoord; // the color variable has attribute position 1
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 orthogonal;
uniform mat4 lightView;

out vec2 texCoord;

vec2 calculateTexCoord() {
    vec4 lightClip = orthogonal * lightView * model * vec4(aPos, 1.0);
    vec2 texC = 0.5 * (lightClip.xy/lightClip.w) + 0.5;
    return texC;
}

void main()
{
    texCoord = calculateTexCoord();
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}   