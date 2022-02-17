#version 330 core
layout (location = 0) in vec3 pos;   // the position variable has attribute position 0
layout (location = 2) in vec2 tex;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec2 texCoord;

void main()
{
    texCoord = tex;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}  
