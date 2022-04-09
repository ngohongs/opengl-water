#version 420 core
layout (location = 0) in vec3 aPosition;  
layout (location = 2) in vec2 aTexCoord; // the color variable has attribute position 1

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 cameraPosition;

layout(binding=0) uniform sampler2D heightField;

out vec2 fTexCoord;
out vec3 fPosition;
out vec3 toCameraDir;

void main()
{
    float height = texture(heightField, aTexCoord).r;
    vec3 offset = vec3(0.0f, height, 0.0f);
    
    vec4 worldCoord = model * vec4(aPosition + offset, 1.0f);
    vec4 projCoord = projection * view * worldCoord;

    fTexCoord = aTexCoord;
    fPosition = worldCoord.xyz;
    toCameraDir = cameraPosition - worldCoord.xyz; 
    
    gl_Position = projCoord;
}   