#version 420 core
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

layout(binding=0) uniform sampler2D causticMap;
layout(binding=1) uniform sampler2D positions;

uniform Light light;
uniform Material material;

uniform vec3 cameraPosition;

uniform float waterHeight;



vec3 dirCalc() {
    float fi = texture(causticMap, texCoord).r;
	if (distance(texture(positions, texCoord).xyz, fPosition) >= 0.1f)
		fi = 0;
	
	float bedHeight = fPosition.y;
	vec3 lightColor;

	float dif = waterHeight - fPosition.y;
	float norm;
	if (dif < 0) {
		lightColor = vec3(1.0f);
		norm = 0;
	}
	else if (dif < .3) {
		lightColor = vec3(2.0f, 204.0f, 147.0f) / 255.0f;
		norm = .3;
	}
	else if (dif < 0.5) {
		lightColor = vec3(2.0f, 127.0f, 199.0f) / 255.0f;
		norm = 0.5 - .3;
	}
	else {
		lightColor = vec3(1.0f, 9.0f, 100.0f) / 255.0f;
		norm = 0.8 - 0.5;
	}
	lightColor = vec3(2.0f, 204.0f, 147.0f) / 255.0f;

	float a = 0.1;
	float I = exp(-a * dif);

	// ambient
	vec3 ambient = light.amb * I * lightColor * material.amb;

	vec3 normal = normalize(fNormal);
	// diffuse
	vec3 toLightDir = normalize(-vec3(light.dir));
	vec3 diffuse =  max(dot(normal, toLightDir), 0.0) * (I + fi)  * mix(light.dif, lightColor, 0.2f) * material.dif;

	// specular
	vec3 fromLightDir = -toLightDir;
	vec3 viewDir = normalize(cameraPosition - fPosition);
	vec3 reflectDir =  reflect(fromLightDir, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shi) * (I + fi) * I * mix(light.spe, lightColor, 0.2f) * material.spe;

	return ambient + diffuse + specular;
}

void main()
{	
	FragColor = vec4(dirCalc(), 1.0);
	//FragColor = vec4(rgb, 1.0);
}