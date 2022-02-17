#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    float len = 2 * length(TexCoords - vec2(0.5));
    float r = mix(1.0, 0, len);
    float b = mix(0.0, 1, len);
    FragColor = vec4(vec3(r, 0, b),1);
}