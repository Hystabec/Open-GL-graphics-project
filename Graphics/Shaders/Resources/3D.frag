#version 420 core

out vec4 colour;

//colour and normal maps
layout (binding = 0) uniform sampler2D tex_colour;
layout (binding = 1) uniform sampler2D tex_normal;
layout (binding = 2) uniform sampler2D texture_ambiant;
layout (binding = 3) uniform sampler2D tex_shadow;

//get the ambient map
//get the light colour

//ambient texture * light colour ?
//diffuse * ^^^ ?
in vec4 FragPosLightSpace;
in vec3 VsNomal;
in vec3 lightPosVs;

in VS_OUT
{
	vec2 texcoord;
	vec3 eyeDir;
	vec3 lightDir;
}	fs_in;

float CalculateShadowValue(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 +0.5;

	float closestDepth = texture(tex_shadow, projCoords.xy).r;

	float currentDepth = projCoords.z;

	float bias = 0.0005;

	/*

	vec3 lightDir = normalize(FragLightPos - FragPos);
	VsNormal = normalize(VsNormal);
	lightDir = normalize(lightDir);
	float bias = max(0.008* (1-dot(VsNormal, -lightDir)), 0.0001);
	float shadow = currentDepth - bias > closestDepth ? 1.0: 0.0;

	*/

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(tex_shadow, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(tex_shadow, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;

	if(projCoords.z >1.0)
	{
		shadow = 0;
	}

	return shadow;
}

void main(void)
{
	float ambientStrength = 0.1f;
	vec3 ambient = vec3(0);

	ambient = ambientStrength * vec3(1);	//this vec3(1) is pure white light could pass in the light colour

	//normalize our incoming view and light direction vectors
	vec3 V = normalize(fs_in.eyeDir);
	vec3 L = normalize(fs_in.lightDir);
	//read the normal form the normal map and normalize it
	vec3 N = normalize(texture(tex_normal, fs_in.texcoord).rgb * 2.0 - vec3(1.0));
	//calculate R ready for use in Phong Lighting
	vec3 R = reflect(-L, N);

	//vec3 normal = N;

	//normal = normalize(normal * 2.0-1.0);
	//normal = normalize(fs_in.TBN*(normal));

	//fetch the diffuse albedo from the texture
	vec3 diffuse_albedo = texture(tex_colour, fs_in.texcoord).rgb;
	//calculate diffuse colour with simple N dot L
	//vec3 diffuse = max(dot(normal,-L), 0.0) * diffuse_albedo;
	vec3 diffuse = vec3(0);
	if(dot(VsNomal, L) >0)
		diffuse = max(dot(N,L), 0.0) * diffuse_albedo;
	//uncomment this to turn off diffuse shading
	//diffuse = vec3(0.0);

	//assume that specular albedo is white; it could also come from a texture
	//vec3 specular_albedo = vec3(0.2);
	vec3 specular_albedo = vec3(1);
	//calculate Phong specular hightlight
	vec3 H = normalize(L + V);
	vec3 specular  = vec3(0);
	if(dot(VsNomal, L) >0)
		specular = pow(max(dot(R, V),0.0), 32.0) * specular_albedo;
		//specular = max(dot(N, H), 0.0) * specular_albedo;
	//vec3 specular = max(pow(dot(R, V), 5.0),0.0) * specular_albedo;
	//uncomment this to turn off specular highlights
	//specular = vec3(0.0);

	//vec3 normal = normalize(texture2D(tex_normal, fs_in.texcoord).rgb);

	

	//final colour is diffuse + specular
	float shadow = CalculateShadowValue(FragPosLightSpace);
	//colour = vec4(diffuse + specular, 1.0);
	colour = vec4((ambient+(1.0-shadow))*(diffuse + specular), 0.0);
	//colour = vec4(specular, 0.0);
	//colour = vec4(diffuse , 1.0);
	//colour = vec4(vec3( VsNomal) , 1.0);
}

/*
    based on code from:
	Sellers G., Wright R.S. and Haemel N. (2015) OpenGL superbible: Comprehensive Tutorial and Reference.
*/