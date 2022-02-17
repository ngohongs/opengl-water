#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;




vec3 calculateTriangleNormal(){
	vec3 tangent = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 bitangent = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 normal = cross(tangent, bitangent);	
	return normalize(normal);
}

const float MAGNITUDE = 0.05;
  
uniform mat4 projection;

void GenerateLine(int index, vec3 normal)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec3 normal = calculateTriangleNormal();

    GenerateLine(0, normal); // first vertex normal
    GenerateLine(1, normal); // second vertex normal
    GenerateLine(2, normal); // third vertex normal
}  
