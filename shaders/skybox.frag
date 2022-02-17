#version 330 core

out vec4 color;

in vec3 TexCoords;

uniform samplerCube tex;


void main()
{    
    color = texture(tex, TexCoords);

}