#version 420 core

struct Light {
	vec3 pos;
	vec3 dir;

	vec3 amb;
	vec3 dif;
	vec3 spe;

};

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform Light light;

layout(binding=0) uniform sampler2D positions;
layout(binding=1) uniform sampler2D normals;
layout(binding=2) uniform sampler2D refractive;


const float ETA = 0.7504;

vec3 EstimateIntersection(vec3 v, vec3 r) {
	vec3 p1 = v + 1.0 * r;
	vec4 texPt = projection * view * vec4(p1, 1.0);
	vec2 texC = 0.5 * (texPt.xy/texPt.w) + 0.5;
	//texC.y = 1 - texC.y;
	vec4 recPos = texture(positions, texC);
	float d = distance(v, recPos.xyz);
	vec3 p2 = v + d * r;
	texPt = projection * view * vec4(p2, 1.0);
	texC = 0.5 * (texPt.xy/texPt.w) + 0.5;
	//texC.y = 1.0 - texC.y;
	return texture(positions, texC).xyz;
}


void main()
{
    // all in world coordinates
	vec3 refractivePosition = texture(refractive, texCoord).rgb;
	vec3 refractiveNormal = normalize(texture(normals, texCoord).rgb);
	vec3 incidentLight = normalize(light.dir);
	vec3 refractedLight  = normalize(refract(incidentLight, refractiveNormal, ETA));

	vec3 p = EstimateIntersection(refractivePosition, refractedLight);
	
	
    gl_Position = projection * view * model * vec4(p, 1.0);
}   