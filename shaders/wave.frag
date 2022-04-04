#version 420 core
out vec4 FragColor;  

in vec3 fPosition;
in vec2 fTexCoord;
in vec3 toCameraDir;
in vec4 fWorldCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float texelSize;


layout(binding=0) uniform sampler2D tex;
layout(binding=1) uniform samplerCube environment;

layout(binding=2) uniform sampler2D positionsRefracted;
layout(binding=4) uniform sampler2D positionsReflected;

layout(binding=3) uniform sampler2D colorRefracted;
layout(binding=5) uniform sampler2D colorReflected;




uniform vec3 cameraPosition;

const float eta = 1.00 / 1.52;


vec2 calculateRefractUV(vec3 normal) {
    vec3 viewDir = normalize(fPosition - cameraPosition);
    vec3 refractDir = refract(viewDir, normal, eta);
    vec3 finalDir = normalize(viewDir + refractDir); // 
    vec4 clipSpaceDir = projection * view * vec4(finalDir, 1.0);
    vec2 texC = 0.5 * (clipSpaceDir.xy/clipSpaceDir.w) + 0.5;
    return texC;
}

vec2 EstimateIntersection(vec3 v, vec3 r, sampler2D positions) {
	vec3 p1 = v + 0.3 * r;
	vec4 texPt = projection * view * vec4(p1, 1.0);
	vec2 texC = 0.5 * (texPt.xy/texPt.w) + 0.5;
	vec4 recPos = texture(positions, texC);
    if (recPos.a <= 0.0f)
        return vec2(-1.0f);
	float d = distance(v, recPos.xyz);
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


    float r = texture(tex, fTexCoord + px).r;
    float l = texture(tex, fTexCoord + nx).r;
    float t = texture(tex, fTexCoord + py).r;
    float b = texture(tex, fTexCoord + ny).r;
    float m = texture(tex, fTexCoord).r;

    vec3 hor = vec3(2 * texelSize ,r - l,0);
    vec3 ver = vec3(0,t - b,2 * texelSize);

    //vec3 normal = normalize(texture(normalMap, fTexCoord).xyz);

    vec3 normal = normalize(cross(ver, hor));

    normal = normalize(mat3(transpose(inverse(model))) * normal);

    //if (cameraPosition.y < fPosition.y)
    //    normal = -normal;

    vec3 viewDir = normalize(fPosition - cameraPosition);
    vec3 reflectDir = normalize(reflect(viewDir, normal));
    vec3 refractDir = normalize(refract(viewDir, normal, eta));

    vec4 reflectColor = texture(environment, reflectDir);
   // vec4 refractColor = texture(environment, refractDir);
	//vec4 refractColor = texture(belowSurface, calculateRefractUV(normal));
    
	//GOOD ------------------
	vec2 refractedUV = EstimateIntersection(fWorldCoord.xyz, refractDir, positionsRefracted);
    vec4 refractedColor = texture(colorRefracted, refractedUV);
    if (refractedUV.x < 0.0f)
        refractedColor = mix(texture(environment, refractDir), texture(environment, viewDir), 0.3);
	//-----------------------

	vec2 reflectedUV = EstimateIntersection(fWorldCoord.xyz, reflectDir, positionsReflected);
    vec4 reflectedColor = texture(colorReflected, reflectedUV);
    if (reflectedUV.x < 0.0f)
        reflectedColor = texture(environment, reflectDir);

    float ci = dot(normal, viewDir);
    float ct = dot(normal, refractDir);

    float n1 = 1.00f;
    float n2 = 1.33f;

    float rs = ((n1*ci-n2*ct)/(n1*ci+n2*ct)) * ((n1*ci-n2*ct)/(n1*ci+n2*ct));
    float rp = ((n2*ct-n1*ci)/(n2*ct+n1*ci)) * ((n2*ct-n1*ci)/(n2*ct+n1*ci));

    float rc = 1 - (rs+rp) / 2;
    float fresnel = dot(normalize(toCameraDir), normal);
    vec4 color = mix(reflectColor, vec4(120.0f/255.0f, 150.0f/255.0f, 233.0f/255.0f, .3f), rc);
    FragColor = mix(reflectedColor, refractedColor, rc);
}