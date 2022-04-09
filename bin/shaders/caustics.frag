#version 420 core
flat in int fValid;
in float fPhi;
in float fDistance;

uniform float causticsPower;
uniform float causticsAbsorbtion;


out vec4 FragColor;

void main()
{   
    if (fValid == 0)
        discard;
    float intensity = fPhi * exp(-causticsAbsorbtion * fDistance) / max((100.0f - causticsPower), 0.001f);
    FragColor = vec4(vec3(intensity), 1.0);
}