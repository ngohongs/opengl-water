#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec2 texCoord;   // the position variable has attribute position 0


out vec4 fPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform bool wave;

uniform sampler2D tex;


void main()
{
    vec4 pos;
    vec4 color;
    if (wave) {
        float height = texture(tex, texCoord).r;
        vec3 offset = vec3(0.0, height, 0.0);

        pos = projection * view * model * vec4(aPos + offset, 1.0);
        color = model * vec4(aPos + offset, 1.0);
    }
    else {
        pos = projection * view * model * vec4(aPos, 1.0);
        color = model * vec4(aPos, 1.0);
    }
    fPos = color;
    gl_Position = pos;
}   