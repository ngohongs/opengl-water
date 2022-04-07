#version 420 core
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0
layout (location = 2) in vec2 aTexCoord;   // the position variable has attribute position 0




uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool wave;

layout(binding=0) uniform sampler2D heightField;

uniform bool under;

uniform float texelSize;

uniform bool duck;
uniform vec3 duckPosition;


out vec4 fPosition;

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

vec3 calcDuckYAxis() {
    vec2 duckUV = 0.5 * duckPosition.xz + 0.5f;

    vec2 px = vec2(texelSize, 0);
    vec2 nx = vec2(-texelSize, 0);
    vec2 py = vec2(0, texelSize);
    vec2 ny = vec2(0, -texelSize);


    float r = texture(heightField, duckUV + px).r;
    float l = texture(heightField, duckUV + nx).r;
    float t = texture(heightField, duckUV + py).r;
    float b = texture(heightField, duckUV + ny).r;
    float m = texture(heightField, duckUV).r;

    vec3 hor = vec3(2 * texelSize ,r - l,0);
    vec3 ver = vec3(0,t - b,2 * texelSize);

    vec3 normal = normalize(cross(ver, hor));
    return normal;
}

mat4 duckRotateMatrix() {
    vec3 target_dir = normalize(calcDuckYAxis());
    vec3 y_axis = vec3(0.0f, 1.0f, 0.0f);
    float rot_angle = acos( dot(target_dir,y_axis) );
    mat4 rotateMatrix = mat4(1.0f);
    if( abs(rot_angle) > 0.001 )
    {
        vec3 rot_axis = normalize( cross(target_dir,y_axis) );
        rotateMatrix = rotate(rotateMatrix, rot_angle, rot_axis);
    }
    return rotateMatrix;
}

float duckHeight() {
    vec2 duckUV = 0.5 * duckPosition.xz + 0.5f;
    return texture(heightField, duckUV).r;
}

void main()
{
    float height = texture(heightField, aTexCoord).r;
    vec3 offset = vec3(0.0, height, 0.0);


    vec4 position; 

    if (wave)
    {
        position = vec4(aPosition + offset, 1.0);
    }
    else
    {
        position = vec4(aPosition, 1.0);
        if (duck)
            position = duckRotateMatrix() * (position + vec4(0.0f, 5.0f * duckHeight(), 0.0f, 0.0f));
    }

    vec4 worldc = model * position;
    vec4 viewc = view * worldc;
    vec4 projc = projection * viewc;
   
    fPosition = worldc;
    

    if (under)
        gl_ClipDistance[0] = dot(worldc, vec4(0,-1,0,0.005));
    else
        gl_ClipDistance[0] = -dot(worldc, vec4(0,-1,0,0.005));

    gl_Position = projc;
}   