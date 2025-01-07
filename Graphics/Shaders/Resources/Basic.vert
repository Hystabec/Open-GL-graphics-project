#version 450
in vec3 VertexPosition;
in vec2 TextureCoord;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;

out vec2 FragTextureCoord;
out vec3 FragNormal;
out vec3 FragPos;
out mat3 TBN;
out int Frag_lightCount;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform int lightCount;
uniform mat4 lightSpaceMatrix;


void main()
{


	vec3 T = normalize(vec3(model* vec4(Tangent, 0.0)));
	vec3 B = normalize(vec3(model* vec4(BiTangent, 0.0)));
	vec3 N = normalize(vec3(model* vec4(Normal, 0.0)));

	TBN = mat3(T,B,N);

	Frag_lightCount = lightCount;
	FragPos = vec3(model* vec4(VertexPosition, 1.0f));
	FragNormal = mat3(transpose(inverse(model))) *Normal;
	FragNormal = normalize(Normal);


	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	FragTextureCoord = TextureCoord;
	gl_Position = perspective * view * model * vec4(VertexPosition, 1.0);
}