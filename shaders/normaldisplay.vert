#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec2 texCoord; // the color variable has attribute position 1
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform sampler2D tex;

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    float height = texture(tex, texCoord).r;
    vec3 offset = vec3(0.0, height, 0.0);
    gl_Position = view * model * vec4(aPos + offset, 1.0);
}   