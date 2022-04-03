#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec2 texCoord; // the color variable has attribute position 1

uniform sampler2D tex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    float height = texture(tex, texCoord).r;
    vec3 offset = vec3(0.0, height, 0.0);
    vec4 worldCoord = model * vec4(aPos + offset, 1.0);
    
    gl_Position = projection * view * worldCoord;
}   