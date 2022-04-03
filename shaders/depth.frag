#version 420 core
out vec4 fragColor;



uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 depth;
in vec3 fviewc;

float near = 0.001; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    float res =  (2.0 * near * far) / (far + near - z * (far - near));
    res = (res+0.3)*(res+0.3) - 0.5;
    return res;
}


void main() {
	float depth = LinearizeDepth(gl_FragCoord.z); // divide by far for demonstration
    fragColor = vec4(vec3(depth), 1.0); 
}