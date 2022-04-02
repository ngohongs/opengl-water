#version 420 core
out vec4 FragColor;  

in vec3 fPosition;
in vec2 fTexCoord;
in vec3 toCameraDir;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float texelSize;


layout(binding=0) uniform sampler2D tex;
layout(binding=1) uniform samplerCube environment;
layout(binding=2) uniform sampler2D belowSurface;

uniform vec3 cameraPosition;

const float eta = 1.00 / 1.52;

vec2 calculateRefractUV(vec3 normal) {
    vec3 viewDir = normalize(fPosition - cameraPosition);
    vec3 refractDir = refract(viewDir, normal, eta);
    vec3 finalDir = normalize(viewDir + refractDir); // 
    vec4 clipSpaceDir = projection * view * vec4(finalDir, 1.0);
    vec2 texC = 0.5 * (clipSpaceDir.xy/clipSpaceDir.w) + 0.5;
    return 1.2 * texC;
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

    if (cameraPosition.y < fPosition.y)
        normal = -normal;

    vec3 viewDir = normalize(fPosition - cameraPosition);
    vec3 reflectDir = normalize(reflect(viewDir, normal));
    vec3 refractDir = normalize(refract(viewDir, normal, eta));

    vec4 reflectColor = texture(environment, reflectDir);
   // vec4 refractColor = texture(environment, refractDir);
    vec4 refractColor = texture(belowSurface, calculateRefractUV(normal));


    float ci = dot(normal, viewDir);
    float ct = dot(normal, refractDir);

    float n1 = 1.00f;
    float n2 = 1.33f;

    float rs = ((n1*ci-n2*ct)/(n1*ci+n2*ct)) * ((n1*ci-n2*ct)/(n1*ci+n2*ct));
    float rp = ((n2*ct-n1*ci)/(n2*ct+n1*ci)) * ((n2*ct-n1*ci)/(n2*ct+n1*ci));

    float rc = 1 - (rs+rp) / 2;
    float fresnel = dot(normalize(toCameraDir), normal);
    vec4 color = mix(reflectColor, refractColor, rc);
    FragColor = mix(reflectColor, vec4(1, 1, 1, 0.15), rc);
}