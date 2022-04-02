#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform sampler2D tex;
uniform float texelSize;

out vec3 fNormal;
out vec2 fTexCoord;

uniform bool wave;

void main()
{
    float height = texture(tex, texCoord).r;
    vec3 offset = vec3(0.0, height, 0.0);

    fTexCoord = texCoord;
    fNormal = aNormal;

    vec4 position; 

    if (wave)
    {
        position = vec4(aPos + offset, 1.0);
    }
    else
    {
        position = vec4(aPos, 1.0);
    }


    gl_Position = projection * view * model * position;
}   