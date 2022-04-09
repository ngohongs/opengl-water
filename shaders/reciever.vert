#version 420 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 orthogonal;
uniform mat4 lightView;
uniform bool duck;
uniform vec3 duckPosition;
uniform float texelSize;
layout(binding=2) uniform sampler2D heightfield;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;
out vec2 fCausticsTexCoord;

 mat4 rotate(mat4 m, float angle, vec3 v) {
	float a = angle;
	float c = cos(a);
	float s = sin(a);

	vec3 axis = normalize(v);
	vec3 temp = ((float(1) - c) * axis);

	mat4 Rotate;
	Rotate[0][0] = c + temp[0] * axis[0];
	Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
	Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

	Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
	Rotate[1][1] = c + temp[1] * axis[1];
	Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

	Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
	Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
	Rotate[2][2] = c + temp[2] * axis[2];

	mat4 Result;
	Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
	Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
	Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
	Result[3] = m[3];
	return Result;
}

vec2 calculateCausticsTexCoord() {
    vec4 lightClip = orthogonal * lightView * model * vec4(aPosition, 1.0);
    vec2 texC = 0.5 * (lightClip.xy/lightClip.w) + 0.5;
    return texC;
}

vec3 calcDuckYAxis() {
    vec2 duckUV = 0.5 * duckPosition.xz + 0.5f;

    vec2 px = vec2(texelSize, 0);
    vec2 nx = vec2(-texelSize, 0);
    vec2 py = vec2(0, texelSize);
    vec2 ny = vec2(0, -texelSize);


    float r = texture(heightfield, duckUV + px).r;
    float l = texture(heightfield, duckUV + nx).r;
    float t = texture(heightfield, duckUV + py).r;
    float b = texture(heightfield, duckUV + ny).r;
    float m = texture(heightfield, duckUV).r;

    vec3 hor = vec3(2 * texelSize ,r - l,0);
    vec3 ver = vec3(0,t - b,2 * texelSize);

    vec3 normal = normalize(cross(ver, hor));
    return normal;
}

mat4 duckRotateMatrix() {
    vec3 targetDir = normalize(calcDuckYAxis());
    vec3 yAxis = vec3(0.0f, 1.0f, 0.0f);
    float rotAngle = acos( dot(targetDir,yAxis) );
    mat4 rotateMatrix = mat4(1.0f);
    if( abs(rotAngle) > 0.001 )
    {
        vec3 rotAxis = normalize( cross(targetDir,yAxis) );
        rotateMatrix = rotate(rotateMatrix, rotAngle, rotAxis);
    }
    return rotateMatrix;
}

float duckHeight() {
    vec2 duckUV = 0.5 * duckPosition.xz + 0.5f;
    return texture(heightfield, duckUV).r;
}

void main()
{
    vec4 position = vec4(aPosition, 1.0f);
    if (duck)
        position = duckRotateMatrix() * (position + vec4(0.0f, 5.0f * duckHeight(), 0.0f, 0.0f));

    vec4 worldCoord = model * position;
    vec4 viewCoord = view * worldCoord;
    vec4 projCoord = projection * viewCoord;

    fPosition = worldCoord.xyz;
    fNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
    fTexCoord = aTexCoord; 
    fCausticsTexCoord = calculateCausticsTexCoord();

    gl_Position = projCoord;
}   