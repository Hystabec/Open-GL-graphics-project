#version 450 core

/*layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4)in vec3 BiTangent;*/

//layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 texcoord;
//layout (location = 2) in vec3 normal;
//layout (location = 3) in vec3 tangent;
in vec3 position;
in vec2 texcoord;
in vec3 normal;
in vec3 tangent;
in vec3 BiTangent;


out VS_OUT
{
	vec2 texcoord;
	vec3 eyeDir;
	vec3 lightDir;
}	vs_out;

out vec4 FragPosLightSpace;
out vec3 VsNomal;
out vec3 lightPosVs;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec3 light_pos;// = vec3(0.0, 0.0, 100.0);
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main(void)
{
	vec3 FragPos = vec3(model * vec4(position, 1.0f));
	vec3 FragNormal = mat3(transpose(inverse(model))) * normal;
	//FragNormal = normal;

	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	//VsNomal =  normal;

	

	//calculate vertex postion in view space
	vec4 P = model * vec4(position,1.0);

	//calculate normal (N) and tangent (T) vectors in view space from
	//incoming object space vector
	vec3 N = normalize(mat3(model) * normal);
	vec3 T = normalize(mat3(model) * tangent);
	//calculate the bitangent vector (B) from the normal and tangent vectors
	vec3 B = cross(N,T);

	//VsNomal= normal;

	//the light vector (L) is the vector from the point of interest to the light
	//calculate that and multiply it by the TBN matrixCompMult
	vec3 L = light_pos - P.xyz;
	lightPosVs = L;
	vs_out.lightDir = normalize(vec3(dot(L, T), dot(L, B), dot(L, N)));
	//vs_out.lightDir = normalize(L);	//overwrites line above

	//the view vector is the vector from the point of interest to the viewer
	//which in view space is simply the negative of the position
	//calculate that and multiply it by the TBN matrix
	vec3 V = -P.xyz;
	vs_out.eyeDir = normalize(vec3(dot(V,T), dot(V,B), dot(V,N)));
	//vs_out.eyeDir = normalize(V);	//overwrites line above

	//vs_out.TBN = mat3(T,B,N);

	VsNomal = normalize(mat3(T,B,N) * normal);

	//pass the texture coordinate through unmodified so that the fragment shader can fetch from the normal and colour maps
	vs_out.texcoord = texcoord;

	//calculate clip coordinates by mutiplting our view position by the projection matrix.
	gl_Position = proj_matrix * mv_matrix* vec4(position,1.0);
	//gl_Position =  model * vec4(position,1.0);
}


/*
    based on code from:
	Sellers G., Wright R.S. and Haemel N. (2015) OpenGL superbible: Comprehensive Tutorial and Reference.
*/