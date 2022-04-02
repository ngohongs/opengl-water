#version 330 core
layout (location = 0) in vec3 pos;   // the position variable has attribute position 0
layout (location = 2) in vec2 texCoord;
uniform sampler2D tex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool wave;

out vec4 fPos;
void main()
{
    vec4 position;
    float offset = texture(tex, texCoord).r;

    if (wave) {
        position = vec4(pos + vec3(0, offset, 0), 1);
    }
    else {
        position = vec4(pos, 1.0);
    }
    fPos = projection * view * model * position;
    gl_Position = fPos;
}