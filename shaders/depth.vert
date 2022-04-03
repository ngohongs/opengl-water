#version 420 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(binding=3) uniform sampler2D tex;
uniform bool wave;

out vec2 fTexCoord;
out vec3 depth;
out vec3 fviewc;
void main()
{

    float height = texture(tex, texCoord).r;
    vec3 offset = vec3(0.0, height, 0.0);


    vec4 position; 

    if (wave)
    {
        position = vec4(aPos + offset, 1.0);
    }
    else
    {
        position = vec4(aPos, 1.0);
    }

    vec4 worldc = model * position;
    vec4 viewc = view * worldc;
    vec4 projc = projection * viewc;

    depth = 0.5 * projc.xyz / projc.w + 0.5;
    fviewc = vec3(viewc);
    gl_Position = projc;
}   