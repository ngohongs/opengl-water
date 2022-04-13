#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 tex;
out vec2 texCoord;
void main()
{
    texCoord = tex;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}