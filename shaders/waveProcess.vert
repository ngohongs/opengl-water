#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec2 texCoord; // the color variable has attribute position 1
out vec2 UV;

void main()
{
    UV = texCoord;
    gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);
}   