#version 420 core
out vec4 FragColor;

flat in int valid;

in float fi;

void main()
{   
    if (valid == 0)
        discard;
    FragColor = vec4(vec3(1.0)*fi/ 30, 1.0);
}