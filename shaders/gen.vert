#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = vec3(mix(0.6, 1.0, aPos.x), mix(0.6, 1.0, aPos.y), mix(0.6, 1.0, aPos.z));
}   