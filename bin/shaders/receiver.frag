#version 420 core  
in vec2 fCausticsTexCoord;
in vec3 fNormal;
in vec3 fPosition;
in vec2 fTexCoord;

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

uniform Light light;
uniform Material material;
uniform float attenuation;
uniform float firstStage;
uniform float secondStage;
uniform vec3 firstStageColor;
uniform vec3 secondStageColor;
uniform vec3 finalStageColor;
uniform vec3 cameraPosition;
uniform float waterHeight;
uniform bool diffuseUsed;

layout(binding=0) uniform sampler2D causticMap;
layout(binding=1) uniform sampler2D positions;
layout(binding=7) uniform sampler2D diffuseTexture;


out vec4 FragColor;  

vec3 dirCalc() {
    float fi = texture(causticMap, fCausticsTexCoord).r;
	if (distance(texture(positions, fCausticsTexCoord).xyz, fPosition) >= 0.1f)
		fi = 0;
	
	float bedHeight = fPosition.y;
	vec3 lightColor;

	float diff = waterHeight - fPosition.y;

	vec3 n = vec3(1.0f);
	vec3 f  =  firstStageColor;
	vec3 s  =  secondStageColor;
	vec3 t  =  finalStageColor;
	
	if (diff <= 0)
		lightColor = n;
	else if (diff <= firstStage)
		lightColor = mix(n, f, (diff - 0.0f) / (firstStage - 0.0f));
	else if (diff <= secondStage)
		lightColor = mix(f, s, (diff - firstStage) / (secondStage - firstStage));
	else
		lightColor = mix(s, t, (diff - secondStage) / (3.0f - secondStage));

	vec3 diffuseColor  = material.dif;
	if (diffuseUsed)
		diffuseColor = texture(diffuseTexture, fTexCoord).rgb;


	float a = attenuation;
	float I = exp(-a * clamp(diff, 0, diff));

	// ambient
	vec3 ambient = light.amb * I * lightColor * material.amb;

	vec3 normal = normalize(fNormal);
	// diffuse
	vec3 toLightDir = normalize(-light.dir);
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