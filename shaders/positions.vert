#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec2 texCoord;   // the position variable has attribute position 0


out vec4 fPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform bool wave;

uniform sampler2D tex;

uniform bool inView;

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

    if (inView) { 
        fPos = viewc;
    }
    else {
        fPos = worldc;
    }
    gl_Position = projc;
}   