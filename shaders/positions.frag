#version 420 core
out vec4 FragColor;

in vec4 fPosition;

void main()
{    
    FragColor = fPosition;
}