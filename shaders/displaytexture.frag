#version 330 core
in vec2 texCoord;
uniform sampler2D tex;
out vec4 fragColor;
uniform bool toggle;

uniform bool depth;
float LinearizeDepth(float depth) 
{
    float far = 100.0f;
    float near = 0.1f;
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    vec4 ref = texture(tex, texCoord);
	fragColor =  ref;
}