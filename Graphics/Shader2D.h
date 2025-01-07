#pragma once
#include <glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>

using namespace std;
class LightBase;
class Transform;
class Camera;
struct lightStruct;

class Shader2D
{
public:
	Shader2D(const string FileLocation, Camera &camera);
	~Shader2D();

	void Update(Transform& transform, std::vector<LightBase*>Lights, glm::mat4 lightSpaceMatrix);
	void UpdateForShadows(Transform& transform, glm::mat4& LightSpaceMatrix);
	void Bind();

	GLuint getProgram();
private:
	lightStruct* convertLightArrayToStruct(std::vector<LightBase*>Lights);

	enum ShaderTypes
	{
		VERTEX,
		FRAGMENT,
		NUM_SHADER_TYPES
	};

	enum UniformNames
	{
		MODEL_U,
		PROJECTION_U,
		VIEW_U,
		//FRAG_LIGHTCOLOR_U,
		//FRAG_LIGHTPOS_U,
		FRAG_CAMERAPOS_U,
		//FRAG_LIGHTARRAY_U,
		FRAG_LIGHTARRAYCOUNT_U,
		LIGHTSPACEMATRIX_U,
		NUM_UNIFORMS
	};

	string Name;
	GLuint m_Program;
	Camera* m_Camera;

	GLuint m_shaders[NUM_SHADER_TYPES];
	GLuint m_uniforms[NUM_UNIFORMS];
	
	GLuint uboLights;

};