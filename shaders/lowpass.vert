#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec2 texCoord; // the color variable has attribute position 1

out vec2 fTexCoord;

void main()
{
    fTexCoord = texCoord;
    gl_Position = vec4(aPos, 1.0);
}   