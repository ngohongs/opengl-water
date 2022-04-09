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
uniform vec3 cameraPosition;
uniform float waterHeight;
uniform bool diffuseUsed = false;
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

	float dif = waterHeight - fPosition.y;

	vec3 n = vec3(1.0f);
	//param--------------------
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
	//-------------
	vec3 diffuseColor;
	if (diffuseUsed)
		diffuseColor = texture(diffuseTexture, fTexCoord).rgb;
	else
		diffuseColor = material.dif;
	//diffuseColor = material.dif;

	//param ---
	float a = 0.0;
	//-----------
	float I = exp(-a * clamp(dif, 0, dif));

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