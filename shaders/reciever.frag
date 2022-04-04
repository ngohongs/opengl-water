#version 330 core
struct Light {
	vec3 pos;
	vec3 dir;

	vec3 amb;
	vec3 dif;
	vec3 spe;

};

struct Material {
    vec3 amb;
	vec3 dif;
	vec3 spe;
    float shi;
}; 



out vec4 FragColor;  
  
in vec2 texCoord;
in vec3 fNormal;
in vec3 fPosition;

uniform sampler2D causticMap;
uniform Light light;
uniform Material material;

uniform vec3 cameraPosition;



vec3 dirCalc() {
    float fi = texture(causticMap, texCoord).r;
	// ambient
	vec3 ambient = light.amb * material.amb;

	vec3 normal = normalize(fNormal);
	// diffuse
	vec3 toLightDir = normalize(-vec3(light.dir));
	vec3 diffuse =  max(dot(normal, toLightDir), 0.0) * (1 + fi) * light.dif * material.dif;

	// specular
	vec3 fromLightDir = -toLightDir;
	vec3 viewDir = normalize(cameraPosition - fPosition);
	vec3 reflectDir =  reflect(fromLightDir, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shi) * (1 + fi) * light.spe * material.spe;

	return ambient + diffuse + specular;
}

void main()
{
	FragColor = vec4(dirCalc(), 1.0);
}