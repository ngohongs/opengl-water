#version 330 core
in vec2 texCoord;
uniform vec2 dropPos;
out vec4 fragColor;

float heightWithDrop(vec2 pos)
{
    float len = length(pos - dropPos);
    return -cos(max(0, 1-10*len))+1;
}

void main()
{
    vec3 color = vec3(heightWithDrop(texCoord));
    fragColor = vec4(color, 1.0);
}
