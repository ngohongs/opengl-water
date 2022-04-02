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

const float ETA = 1.00 / 1.33;

flat out int valid;

uniform int v;

out float fi;

vec3 EstimateIntersection(vec3 v, vec3 r) {
	vec3 p1 = v + 1.0 * r;
	vec4 texPt = projection * view * vec4(p1, 1.0);
	vec2 texC = 0.5 * (texPt.xy/texPt.w) + 0.5;
	vec4 recPos = texture(positions, texC);
	float d = distance(v, recPos.xyz);
	vec3 p2 = v + d * r;
	texPt = projection * view * vec4(p2, 1.0);
	texC = 0.5 * (texPt.xy/texPt.w) + 0.5;
	return texture(positions, texC).xyz;
}

void main()
{
    // all in world coordinates
	vec3 refractivePosition = texture(refractive, texCoord).rgb;
	vec3 refractiveNormal = texture(normals, texCoord).rgb;
	//vec3 incidentLight = normalize(refractivePosition - light.pos);
	vec3 incidentLight = normalize(light.dir);
	vec3 refractedLight  = normalize(refract(incidentLight, refractiveNormal, ETA));

	vec3 p = EstimateIntersection(refractivePosition, refractedLight);

	if (distance(refractiveNormal, vec3(0)) < 0.001 || p.xyz == vec3(1.0f) || dot(p - refractivePosition, refractedLight) < 0.1)
		valid = 0;
	else
		valid = 1;

	fi = dot(-incidentLight, refractiveNormal);
    gl_Position = projection * view * model * vec4(p, 1.0);
}   