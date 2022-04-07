#version 330 core
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform sampler2D heightField;
uniform float texelSize;

out vec3 fNormal;
out vec2 fTexCoord;

uniform bool wave;

void main()
{
    float height = texture(heightField, aTexCoord).r;
    vec4 offset = vec4(0.0f, height, 0.0f, 0.0f);

    fTexCoord = aTexCoord;
    fNormal = aNormal;

    vec4 position = vec4(aPosition, 1.0f);

    if (wave)
        position += offset;
  

    gl_Position = projection * view * model * position;
}   