#version 330 core
in vec2 texCoord;
uniform sampler2D tex;
out vec4 fragColor;
uniform bool toggle;

void main()
{
	fragColor =  texture(tex, texCoord);
}