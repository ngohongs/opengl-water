#version 330

/**
 * Take in triangles for calculating normals
 */
layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 3) out;

/**
 * Vertex output - fragment inputs
 * 
 *	    fPosition - position of the vertex in camera space
 *		  fNormal - normal of the vertex
 *		fTexCoord - texturing coordinates of vertex
 */
out vec3 fNormal;

/**
 * Pojection, view, model matrices
 */
/**
 * Fog constants
 */
/**
 * Calculation of triangle's normal
 */ 
vec3 calculateTriangleNormal(){
	vec3 tangent = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 bitangent = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 normal = cross(tangent, bitangent);	
	return normalize(normal);
}


void main(void){
	vec3 normal = calculateTriangleNormal();

	vec4 position = gl_in[0].gl_Position;
	gl_Position = vec4(position.xz, 0, 1);
	fNormal = normal;
	EmitVertex();
	
	position = gl_in[1].gl_Position;
	gl_Position = vec4(position.xz, 0, 1);
	fNormal = normal;
	EmitVertex();
	
	position = gl_in[2].gl_Position;
	gl_Position = vec4(position.xz, 0, 1);
	fNormal = normal;
	EmitVertex();
	
	EndPrimitive();
}