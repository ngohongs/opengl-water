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
    float radius = 1.5 * texelSize;
	float val = max(0.0, 1.0 - length(dropPos - pos) / radius);
    val = 0.5 - cos(val * PI) * 0.5;
    return  val;
}


void main()
{
    vec4 heightField = texture(tex, UV);
    if (-0.1 > heightField.r || heightField.r > 0.1) {
        FragColor = heightField;
        return;
    }
    heightField.r += height(UV);
    heightField.r = clamp(heightField.r, -0.05, 0.05);
    FragColor = heightField;
}