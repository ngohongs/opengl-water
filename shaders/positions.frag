#version 330 core
out vec4 FragColor;

in vec4 fPos;

void main()
{    
    FragColor = fPos;
}