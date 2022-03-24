#version 330 core
out vec4 FragColor;  
in vec2 UV;
uniform sampler2D tex;
uniform float texelSize;
uniform bool drop;
uniform vec2 dropPos;


const float PI = 3.14159265359;


float height(vec2 pos)
{
    if (!drop)
        return 0.0;
    float radius = 3 * texelSize;
	float val = max(0.0, 1.0 - length(dropPos - pos) / radius);
    val = 0.5 - cos(val * PI) * 0.5;
    return 0.5 * val;
}


void main()
{
    vec4 heightField = texture(tex, UV);
    heightField.r += height(UV);
    FragColor = heightField;
}