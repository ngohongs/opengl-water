#version 330 core
out vec4 FragColor;  
  
uniform sampler2D causticMap;
in vec2 texCoord;




void main()
{
    vec3 add = texture(causticMap, texCoord).rgb;
    vec4 addC = vec4(add, 0.0);
    FragColor = vec4(0, 0, 0, 1.0) + texture(causticMap, texCoord);
}