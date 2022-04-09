#version 420 core
layout (location = 2) in vec2 aTexCoord;

struct Light {
	vec3 pos;
	vec3 dir;

	vec3 amb;
	vec3 dif;
	vec3 spe;

};

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform Light light;

layout(binding=0) uniform sampler2D recieverPositions;
layout(binding=1) uniform sampler2D refractiveNormals;
layout(binding=2) uniform sampler2D refractivePositions;

const float ETA = 1.00 / 1.33;

flat out int fValid;
out float fPhi;
out float fDistance;

vec4 EstimateIntersection(vec3 v, vec3 r) {
	vec3 p1 = v + 1.0 * r;
	vec4 texPt = projection * view * vec4(p1, 1.0);
	vec2 texC = 0.5 * (texPt.xy/texPt.w) + 0.5;
	vec4 recPos = texture(recieverPositions, texC);
	float d = distance(v, recPos.xyz);
	vec3 p2 = v + d * r;
	texPt = projection * view * vec4(p2, 1.0);
	texC = 0.5 * (texPt.xy/texPt.w) + 0.5;
	return texture(recieverPositions, texC);
}

void main()
{
	vec4 refractivePosition = texture(refractivePositions, aTexCoord);
	vec4 refractiveNormal = texture(refractiveNormals, aTexCoord);
	vec3 incidentLight = normalize(light.dir);
	vec3 refractedLight  = normalize(refract(incidentLight, normalize(refractiveNormal.xyz), ETA));

	vec4 p = EstimateIntersection(refractivePosition.xyz, refractedLight);

	if (refractivePosition.a <= 0.001f || refractiveNormal.a <= 0.001f || p.a <= 0.001f  || p.y > refractivePosition.y || dot(p.xyz - refractivePosition.xyz, refractedLight) < 0.01f )
		fValid = 0;
	else
		fValid = 1;

	fDistance = distance(p, refractivePosition);
	fPhi = dot(-incidentLight, normalize(refractiveNormal.xyz));
    gl_Position = projection * view * model * vec4(p.xyz, 1.0);
}   