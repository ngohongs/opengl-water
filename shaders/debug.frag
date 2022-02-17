#version 330 core
in vec2 texCoord;
uniform sampler2D tex;
out vec4 fragColor;
uniform bool toggle;

void main()
{
	vec4 rgba = texture(tex, texCoord);
	float r = rgba.r;
	float g = rgba.g;
	if (toggle)
	{
		if (g >= 0)
			fragColor = vec4(0, g, 0, 1);
		if (g < 0)
			fragColor = vec4(0, 0, -g, 1);
	}
	else
		fragColor = vec4(r, 0, 0, 1);
}
