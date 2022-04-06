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

layout(binding=7) uniform sampler2D diffuseTexture;

uniform bool diffuseUsed = false;

in vec2 uv;




vec3 dirCalc() {
    float fi = texture(causticMap, texCoord).r;
	if (distance(texture(positions, texCoord).xyz, fPosition) >= 0.1f)
		fi = 0;
	
	float bedHeight = fPosition.y;
	vec3 lightColor;

	float dif = waterHeight - fPosition.y;
	float norm;

	vec3 n = vec3(1.0f);
	vec3 f  = vec3(2.0f, 204.0f, 147.0f) / 255.0f;
	vec3 s  = vec3(2.0f, 204.0f, 147.0f) / 255.0f;
	vec3 t  = vec3(2.0f, 204.0f, 147.0f) / 255.0f;

	if (dif <= 0) {
		lightColor = n;
	}
	else if (dif <= .3) {
		lightColor = mix(n,f,dif / 0.3);
	}
	else if (dif <= 0.5) {
		lightColor = mix(f,s,(dif-0.3)/0.2);
	}
	else {
		lightColor = mix(s,t,(dif-0.5)/2.5f);
	}

	vec3 diffuseColor;
	if (diffuseUsed)
		diffuseColor = texture(diffuseTexture, uv).rgb;
	else
		diffuseColor = material.dif;
		

	float a = 1.0;
	float I = exp(-a * clamp(dif, 0, dif));

	// ambient
	vec3 ambient = light.amb * I * lightColor * material.amb;

	vec3 normal = normalize(fNormal);
	// diffuse
	vec3 toLightDir = normalize(-vec3(light.dir));
	vec3 diffuse =  max(dot(normal, toLightDir), 0.0) * (I + fi)  * light.dif * lightColor * diffuseColor;

	// specular
	vec3 fromLightDir = -toLightDir;
	vec3 viewDir = normalize(cameraPosition - fPosition);
	vec3 reflectDir =  reflect(fromLightDir, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shi) * (I + fi) * I * light.spe * lightColor * material.spe;

	return ambient + diffuse + specular;
}

void main()
{	
	FragColor = vec4(dirCalc(), 1.0);
}