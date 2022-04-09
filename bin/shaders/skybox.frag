#version 330 core

out vec4 FragColor;

in vec3 fTexCoords;

uniform samplerCube skyboxTexture;


void main()
{    
    FragColor = texture(skyboxTexture, fTexCoords);
}