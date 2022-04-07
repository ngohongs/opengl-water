#version 420 core
in vec2 fTexCoord;

layout(binding=7) uniform sampler2D textureImage;

out vec4 FragColor;

void main()
{
	FragColor = texture(textureImage, fTexCoord);
}