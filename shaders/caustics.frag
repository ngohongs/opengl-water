#version 420 core
out vec4 FragColor;

flat in int valid;

in float fi;
in float d;
void main()
{   
    if (valid == 0)
        discard;
    float p = 10;
    float a = 1.0f;
    float I = fi / p * exp(-a * d);
    FragColor = vec4(vec3(1.0) * I, 1.0);
}