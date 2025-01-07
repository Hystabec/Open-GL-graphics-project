#include "Shader.h"
#include "LightBase.h"
#include "Transforms.hpp"
#include "Camera.hpp"
#include "LightStruct.h"


static string LoadShader(const string& fileName)
{
	std::ifstream file;
	const char* fileNameChar = fileName.c_str();
	file.open(fileNameChar, ifstream::in);

	string output;
	string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		cerr << "Unable to load shader: " << fileName << endl;
	}

	file.close();

	return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		cerr << errorMessage << ": '" << error << "'" << endl;
	}
}

static GLuint CreateShader(const string& ShaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		cerr << "ERROR: shader creation failed!" << endl;

	//needed to convert from string to const char* which glShaderSource wants.
	const char* tempSourceCode = ShaderSource.c_str();

	glShaderSource(shader, 1, &tempSourceCode, NULL);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: shader compilation failed: ");
	return shader;
}

Shader::Shader(const string FileLocation, Camera& camera)
{
	m_Camera = &camera;
	Name = FileLocation;
	m_Program = glCreateProgram();

	m_shaders[VERTEX] = CreateShader(LoadShader(FileLocation + ".vert"), GL_VERTEX_SHADER);
	m_shaders[FRAGMENT] = CreateShader(LoadShader(FileLocation + ".frag"), GL_FRAGMENT_SHADER);

	for (GLuint i = 0; i < NUM_SHADER_TYPES; i++)
	{
		glAttachShader(m_Program, m_shaders[i]);
	}

	glLinkProgram(m_Program);
	CheckShaderError(m_Program, GL_LINK_STATUS, true, "ERROR: program linking failed: ");
	glValidateProgram(m_Program);
	CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "ERROR: program is invlide: ");

	//m_uniforms[MODEL_U]				 =		 glGetUniformLocation(m_Program, "model");
	//m_uniforms[PROJECTION_U]		 =		 glGetUniformLocation(m_Program, "perspective");
	//m_uniforms[VIEW_U]				 =		 glGetUniformLocation(m_Program, "view");
	
	/*m_uniforms[FRAG_LIGHTCOLOR_U]	 =		 glGetUniformLocation(m_Program, "FragLightColor");
	m_uniforms[FRAG_LIGHTPOS_U]		 =		 glGetUniformLocation(m_Program, "FragLightPos");*/
	//m_uniforms[FRAG_CAMERAPOS_U]	 =		 glGetUniformLocation(m_Program, "FragCamPos");
	
	//m_uniforms[FRAG_LIGHTARRAYCOUNT_U]	 =	 glGetUniformLocation(m_Program, "lightCount");

	m_uniforms[MV_MATRIX_U] = glGetUniformLocation(m_Program, "mv_matrix");
	m_uniforms[PROJECTION_U] = glGetUniformLocation(m_Program, "proj_matrix");
	m_uniforms[FRAG_LIGHTPOS_U] = glGetUniformLocation(m_Program, "light_pos");
	m_uniforms[MODEL_U] = glGetUniformLocation(m_Program, "model");
	m_uniforms[LIGHTSPACEMATRIX_U] = glGetUniformLocation(m_Program, "lightSpaceMatrix");
	//m_uniforms[VIEW_U] = glGetUniformLocation(m_Program, "view");

	//m_uniforms[FRAG_LIGHTARRAY_U]	=		 glGetUniformLocation(m_Program, "lights");

	for (GLuint i = 0; i < NUM_UNIFORMS; i++)
	{
		if (m_uniforms[i] == GL_INVALID_INDEX)
			cout << "Shader " << Name << " Uniform invalid index:" << static_cast<UniformNames>(i)
			<< " (Might be optimized if not used)" << endl;
	}



	GLuint uniformBlockIndexLights = glGetUniformBlockIndex(m_Program, "LightBlock");
	
	glGenBuffers(1, &uboLights);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lightStruct)*20 , NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndexLights, uboLights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADER_TYPES; i++)
	{
		glDetachShader(m_Program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteProgram(m_Program);
}

void Shader::Update(Transform& transform, std::vector<LightBase*>Lights, mat4 lightSpaceMatrix)
{
	mat4 projection = m_Camera->m_perspectiveProjection;
	mat4 view = m_Camera->viewMatrix();
	mat4 model = transform.GetModel();
		
	mat4 Mv_matrix = view * model;
	glUniformMatrix4fv(m_uniforms[MV_MATRIX_U], 1, GL_FALSE, &Mv_matrix[0][0]);
	glUniformMatrix4fv(m_uniforms[PROJECTION_U], 1, GL_FALSE, &projection[0][0]);
	//glUniformMatrix4fv(m_uniforms[VIEW_U], 1, GL_FALSE, &view[0][0]);
	//glUniform1i(m_uniforms[FRAG_LIGHTARRAYCOUNT_U], Lights.size());
	glUniform3f(m_uniforms[FRAG_LIGHTPOS_U], Lights[0]->getTransform().getPosition().x,
		Lights[0]->getTransform().getPosition().y,
		Lights[0]->getTransform().getPosition().z);

	glUniformMatrix4fv(m_uniforms[MODEL_U], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m_uniforms[LIGHTSPACEMATRIX_U], 1, GL_FALSE, &lightSpaceMatrix[0][0]);
	/*glUniform3f(m_uniforms[FRAG_CAMERAPOS_U], m_Camera->m_transform.getPosition().x,
		m_Camera->m_transform.getPosition().y,
		m_Camera->m_transform.getPosition().z);*/

	//pass in the lightStruct array and create a struct in the frag shader iterate over each element in the array
	


	/*glUniform3f(m_uniforms[FRAG_LIGHTCOLOR_U], light.getColour().x,
		light.getColour().y,
		light.getColour().z);*/
	int s = Lights.size();

	//glUniform1f(m_uniforms[FRAG_LIGHTARRAYCOUNT_U], Lights.size());	//pass in the length of the array

	lightStruct* lights = convertLightArrayToStruct(Lights);

	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightStruct)*20 , lights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	delete[] lights;
}

void Shader::UpdateForShadows(Transform& transform, mat4& LightSpaceMatrix)
{
	//pass in a vector<Transfrom&> the loop over each or create a struct to send to GPU

	glUniformMatrix4fv(glGetUniformLocation(m_Program, "model"), 1, GL_FALSE, &transform.GetModel()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Program, "lightSpaceMatrix"), 1, GL_FALSE, &LightSpaceMatrix[0][0]);

}

void Shader::Bind()
{
	glUseProgram(m_Program);
}

GLuint Shader::getProgram()
{
	return m_Program;
}

lightStruct* Shader::convertLightArrayToStruct(std::vector<LightBase*>Lights)
{
	lightStruct* NewLights = new lightStruct[Lights.size()];
	for (int i = 0; i < Lights.size(); i++)
	{
		NewLights[i].m_Colour = Lights[i]->getColour();
		NewLights[i].Pos = Lights[i]->getTransform().getPosition();
		NewLights[i].length = Lights.size();
	}

	return NewLights;
}
