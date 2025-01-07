#version 450

in vec3 FragNormal;
in vec3 FragPos;
in vec2 FragTextureCoord;
in mat3 TBN;


flat in int Frag_lightCount;

in vec4 FragPosLightSpace;


out vec4 frag_colour;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_shadow;


//uniform vec3 FragLightColor;
//uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

struct lightStruct
{
	vec3 Pos;
	int m_length;
	vec3 m_Colour;
	float padding2Paddington;
	
};

layout (std140) uniform LightBlock{
lightStruct lights[20];
};


float CalculateShadowValue(vec4 fragPosLightSpace)
{
	vec3 projCoords =  fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 +0.5;

	float closestDepth = texture(texture_shadow, projCoords.xy).r;

	float currentDepth = projCoords.z;

	float bias = 0.0005;

	

//	vec3 lightDir = normalize(lights[0].Pos - FragPos);
//	vec3 VsNormal = normalize(FragNormal);
//	lightDir = normalize(lightDir);
//	float bias = max(0.008* (1-dot(VsNormal, -lightDir)), 0.0001);
//	float shadow = currentDepth - bias > closestDepth ? 1.0: 0.0;

	

	float shadow = 0.0;
	
	vec2 texelSize = 1.0 / textureSize(texture_shadow, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(texture_shadow, projCoords.xy + vec2(x, y) * texelSize).r;
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

//"uniform vec3 Color;"
void main()
{
	//frag_colour = vec4(vec3(FragTextureCoord.x, FragTextureCoord.y, 0),1.0f);
	//frag_colour = vec4(vec3(1,0,0),1.0f);
	//frag_colour = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb,1);


	float ambientStrenght = 0.5f;
	vec3 ambient=vec3(0);
	//vec3 normal = normalize(FragNormal);

	vec3 normal = normalize(texture2D(texture_normal, FragTextureCoord).rgb);
	
	normal = normalize(normal * 2.0-1.0);
	normal = normalize(TBN*(normal*FragNormal));

	vec3 diffuse  =vec3(0);
	float specularStrength = 2f;
	vec3 viewDir = normalize(FragCamPos - FragPos);
	vec3 specular = vec3(0);

	for(int i=0; i< lights[0].m_length;i++ )
	{
		ambient += ambientStrenght * lights[i].m_Colour;
		
		vec3 lightDir = -normalize(lights[i].Pos*2 - FragPos);

		float diff = max(dot(normal,  -lightDir),0.0);

		diffuse += diff * lights[i].m_Colour;

		//if(dot(FragNormal, -lightDir)>= 0)
		//	diffuse += diff * lights[i].m_Colour;

		vec3 reflectDir = reflect(-lightDir, normal);

		float spec = pow(max(dot(normal, reflectDir), 0.0), 16.0);

		specular += vec3(specularStrength * spec);

		//if(dot(FragNormal, -lightDir)>= 0)
		//	specular += vec3(specularStrength * spec);
	}

	//diffuse
	
	//specular
	float shadow = CalculateShadowValue(FragPosLightSpace);
	//(1.0-shadow)
	//vec4 result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb  * (ambient + (1.0 -shadow)) * (diffuse*specular),0);
	vec4 result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb  * (ambient+ diffuse) *(1.0-shadow*5) + specular, 1);
	///result = vec4(texture2D(texture_diffuse,FragTextureCoord).rgb * (ambient+diffuse) *(1.0-shadow*5), 1);
	//result = vec4(vec3(1.0-shadow*5), 1);
	//result= vec4((diffuse*specular), 1);
	frag_colour = result ;

	//(ambient+ diffuse) *(1.0-shadow*5) + specular, 1);
}