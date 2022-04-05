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

layout(binding=6) uniform sampler2D screenDepth;


uniform vec3 cameraPosition;

const float eta = 1.00 / 1.52;

float LinearizeDepth(float depth) 
{
    float far = 100.0f;
    float near = 0.1f;
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec2 calculateRefractUV(vec3 normal) {
    vec3 viewDir = normalize(fPosition - cameraPosition);
    vec3 refractDir = refract(viewDir, normal, eta);
    vec3 finalDir = normalize(viewDir + refractDir); // 
    vec4 clipSpaceDir = projection * view * vec4(finalDir, 1.0);
    vec2 texC = 0.5 * (clipSpaceDir.xy/clipSpaceDir.w) + 0.5;
    return texC;
}

vec2 EstimateIntersection(vec3 v, vec3 r, vec3 normal, sampler2D positions) {
	vec3 p1 = v + 0.1 * (1 - dot(normal, normalize(toCameraDir))) * r;
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


vec3 getScreenPos (vec3 worldPos) {
	vec4 screenPos 	= projection * view * vec4(worldPos, 1.0);
	vec3 screenPos3	= screenPos.xyz / screenPos.w;
	return (screenPos3 + vec3(1.0)) / 2.0;
}

vec3 raytrace(in vec3 reflectionWorld, in int maxCount, in float stepSize) {
	vec3 color = vec3(1.0); 	
	vec3 testVec = fWorldCoord.xyz;
	
	vec3 reflectionVector = reflectionWorld * stepSize;
	
	vec3 screenPos		= getScreenPos(testVec);
	vec2 screenTexPos 	= screenPos.xy;
	
	
	float texDepth = LinearizeDepth(texture(screenDepth, screenTexPos).x);
	float worldDepth = LinearizeDepth(screenPos.z);
	
	bool run = true;
	int count = 0;

	while (run) {
		texDepth = texture(screenDepth, screenTexPos).x;
		float mask = texture(colorRefracted, screenTexPos).a;
		worldDepth = screenPos.z;
		
		
		if (texDepth <= worldDepth) {
			color = texture(colorRefracted, screenTexPos).rgb;
			break;
		}

		
		testVec 		= testVec + reflectionVector;
		screenPos		= getScreenPos(testVec);
		screenTexPos 	= screenPos.xy;
		
		count = count+1;
		run = 	screenTexPos.x < 1.0 && screenTexPos.x >= 0.0 &&
				screenTexPos.y < 1.0 && screenTexPos.y >= 0.0 && count < maxCount;
	}
	
	if (!run) {
		color = texture(environment, reflectionWorld).rgb;

	}
	
	return color;
} 

float calcStepSize(float stepSize, float max, vec3 n, vec3 cam) {
	float scalarProduct = dot(-n, cam);
	float res = 0.0;
	
	res = (1.0 - scalarProduct) * (max-stepSize);
	
	return res + stepSize;
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
	vec2 refractedUV = EstimateIntersection(fWorldCoord.xyz, refractDir, normal, positionsRefracted);
    vec4 refractedColor = texture(colorRefracted, refractedUV);
    if (refractedUV.x < 0.0f)
        refractedColor = mix(texture(environment, refractDir), texture(environment, viewDir), 0.3);
	//-----------------------

	vec2 reflectedUV = EstimateIntersection(fWorldCoord.xyz, reflectDir, normal, positionsReflected);
    vec4 reflectedColor = texture(colorReflected, reflectedUV);
    if (reflectedUV.x < 0.0f)
        reflectedColor = texture(environment, reflectDir);

	const int maxCount = 250;
	float stepSize = 0.01;
	float maxStepSize = 0.1;
	//stepSize = calcStepSize(stepSize, maxStepSize , normal, viewDir);
	vec3 testr = raytrace(refractDir, maxCount, stepSize);
	vec3 testl = raytrace(reflectDir, maxCount, stepSize);

    float ci = dot(normal, viewDir);
    float ct = dot(normal, refractDir);

    float n1 = 1.00f;
    float n2 = 1.33f;

    float rs = ((n1*ci-n2*ct)/(n1*ci+n2*ct)) * ((n1*ci-n2*ct)/(n1*ci+n2*ct));
    float rp = ((n2*ct-n1*ci)/(n2*ct+n1*ci)) * ((n2*ct-n1*ci)/(n2*ct+n1*ci));

    float rc = 1 - (rs+rp) / 2;
    float fresnel = dot(normalize(toCameraDir), normal);
    vec3 color = mix(reflectedColor, refractedColor, rc).rgb;
	vec3 test = mix(testl, testr, rc);
	vec3 waterColor = vec3(2.0f, 204.0f, 147.0f) / 255.0f;
    FragColor = vec4(vec3(mix(color, waterColor, 0.025f)), 1.0f);
}