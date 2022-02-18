#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0

out vec4 fPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    fPos = pos;
    gl_Position = pos;
}   