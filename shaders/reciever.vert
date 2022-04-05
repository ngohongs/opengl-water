#version 420 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoord; // the color variable has attribute position 1
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 orthogonal;
uniform mat4 lightView;


out vec2 texCoord;

out vec3 fPosition;
out vec3 fNormal;
out float bedHeight;

uniform bool under;
vec2 calculateTexCoord() {
    vec4 lightClip = orthogonal * lightView * model * vec4(aPos, 1.0);
    vec2 texC = 0.5 * (lightClip.xy/lightClip.w) + 0.5;
    return texC;
}

void main()
{
    texCoord = calculateTexCoord();

    vec4 worldCoord = model * vec4(aPos, 1.0);
    vec4 viewCoord = view * worldCoord;
    vec4 projCoord = projection * viewCoord;

    fNormal = mat3(transpose(inverse(model)))  * aNormal;

    fPosition = worldCoord.xyz;
    if (under)
        gl_ClipDistance[0] = dot(worldCoord, vec4(0,-1,0,0));
    else
        gl_ClipDistance[0] = dot(worldCoord, vec4(0,1,0,0));

    gl_Position = projCoord;
}   