#version 420 core
in vec3 fPosition;
in vec2 fTexCoord;
in vec3 toCameraDir;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float texelSize;
uniform float firstGuess;

uniform vec3 cameraPosition;
uniform vec3 waterColor;

layout(binding=0) uniform sampler2D heightField;
layout(binding=1) uniform samplerCube environment;
layout(binding=2) uniform sampler2D positionsRefracted;
layout(binding=3) uniform sampler2D colorRefracted;
layout(binding=4) uniform sampler2D positionsReflected;
layout(binding=5) uniform sampler2D colorReflected;

out vec4 FragColor; 

const float eta = 1.00 / 1.52;

vec2 EstimateIntersection(vec3 v, vec3 r, vec3 normal, sampler2D positions) {
	vec3 p1 = v + firstGuess * (1 - dot(normal, normalize(toCameraDir))) * r; 
	vec4 texPt = projection * view * vec4(p1, 1.0);
	vec2 texC = 0.5 * (texPt.xy/texPt.w) + 0.5;
	vec4 recPos = texture(positions, texC);
    if (recPos.a <= 0.0f)
        return vec2(-1.0f);
	float d = distance(v, recPos.xyz) * (1 - dot(normal, normalize(toCameraDir)));
	vec3 p2 = v + d * r;
	texPt = projection * view * vec4(p2, 1.0);
	texC = 0.5 * (texPt.xy/texPt.w) + 0.5;

    if (texC.x < 0 || texC.x > 1 || texC.y < 0 || texC.y > 1)
        return vec2(-1.0f);
	return texC;
}


void main()
{    
    vec2 px = vec2(texelSize, 0);
    vec2 nx = vec2(-texelSize, 0);
    vec2 py = vec2(0, texelSize);
    vec2 ny = vec2(0, -texelSize);


    float r = texture(heightField, fTexCoord + px).r;
    float l = texture(heightField, fTexCoord + nx).r;
    float t = texture(heightField, fTexCoord + py).r;
    float b = texture(heightField, fTexCoord + ny).r;

    vec3 hor = vec3(2 * texelSize ,r - l,0);
    vec3 ver = vec3(0,t - b,2 * texelSize);

    vec3 normal = normalize(cross(ver, hor));
    normal = normalize(mat3(transpose(inverse(model))) * normal);

    if (cameraPosition.y < fPosition.y)
        normal = -normal;

    vec3 viewDir = normalize(fPosition - cameraPosition);
    vec3 reflectDir = normalize(reflect(viewDir, normal));
    vec3 refractDir = normalize(refract(viewDir, normal, eta));

    
	vec2 refractedUV = EstimateIntersection(fPosition, refractDir, normal, positionsRefracted);
    vec4 refractedColor = texture(colorRefracted, refractedUV);
    if (refractedUV.x <= 0.0f)
       refractedColor = texture(environment, refractDir);


	vec2 reflectedUV = EstimateIntersection(fPosition, reflectDir, normal, positionsReflected);
    vec4 reflectedColor = texture(colorReflected, reflectedUV);
    if (reflectedUV.x <= 0.0f)
        reflectedColor = texture(environment, reflectDir);

    float ci = dot(normal, viewDir);
    float ct = dot(normal, refractDir);

    float n1 = 1.00f;
    float n2 = 1.33f;

    float rs = ((n1*ci-n2*ct)/(n1*ci+n2*ct)) * ((n1*ci-n2*ct)/(n1*ci+n2*ct));
    float rp = ((n2*ct-n1*ci)/(n2*ct+n1*ci)) * ((n2*ct-n1*ci)/(n2*ct+n1*ci));

    float rc = 1 - (rs+rp) / 2;

    vec3 color = mix(reflectedColor, refractedColor, rc).rgb;
	vec3 waterColor = vec3(2.0f, 204.0f, 147.0f) / 255.0f;
    FragColor = vec4(vec3(mix(color, waterColor, 0.025f)), 1.0f);
}