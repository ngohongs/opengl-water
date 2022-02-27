#version 420 core
out vec4 FragColor;

flat in int valid;

in float fi;

void main()
{   
    FragColor = valid == 1 ? vec4(vec3(1.0), 0.02) : vec4(0.0);
}