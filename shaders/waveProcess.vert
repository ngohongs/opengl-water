#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 2) in vec2 aTexCoord; 
out vec2 UV;

void main()
{
    UV = aTexCoord;
    gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);
}   