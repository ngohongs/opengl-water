#version 330 core
in vec4 fPos;

out vec4 fragColor;
void main()
{
    fragColor = vec4(fPos.xyz/fPos.w, 1.0);
}
