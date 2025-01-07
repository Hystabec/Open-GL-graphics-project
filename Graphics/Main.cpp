#define GLEW_STATIC
//#define STB_IMAGE_IMPLEMENTATION

#include <glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>	
#include "Mesh.h"
#include "Transforms.hpp"
#include "Vertex.h"
#include <vector>
#include "Camera.hpp"
#include "Input.hpp"
#include "Shader.h"
#include "Shader2D.h"

#include "Texture2D.hpp"

#include "LightBase.h"
#include "OBJLoader.h"

//#include "stb_image.h"

#include <iostream>

using namespace std;


mat4 createLightSpaceMatrix(float near_plane, float far_plane, vector<LightBase*> lights)
{
	//this will need to loop over all the ligths in the vector if i want to expand it to work with more lights

	mat4 lightProjection = ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
	mat4 lightView = lookAt(lights[0]->getTransform().getPosition(), vec3(0), vec3(0, 1, 0));

	mat4 lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}

int main(int argc, char* argv[])
{	
	//my world cords are flipped    x++; goes left (i think it should go right)


	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	int windowWidth = 800;
	int windowHeight = 600;

	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SKIP_TASKBAR;

	SDL_Window* window = SDL_CreateWindow("Graphics window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight, flags);
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);

	/*

	SDL_Surface *icon;						//sets up an icon
	icon = SDL_LoadBMP("../Graphics/assets/Frampt.bmp");		//sets the icon the be a loaded bitmap

	if (icon == NULL)						//if there is no icon loaded, give an error 
	{
		cout << "Error loading icon\n";
		cout << SDL_GetError();
	}
	SDL_SetWindowIcon(window, icon);		//sets the window icon to be the loaded bitmap
	SDL_FreeSurface(icon);					//deloads the icon, so it isnt permenetly stored in memory

	*/

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (status != GLEW_OK)
	{
		cout << "GLEW failed to intialize" << endl;
	}

	//glDepthRange(1,0);		//this inverts the coordinate system


	//triangle 1
	vector<Vertex> TriangleVerticies;
	TriangleVerticies.push_back(Vertex(0.0f, 0.0f, 0.0f));
	TriangleVerticies.push_back(Vertex(0.5f, -1.0f, 0.0f));
	TriangleVerticies.push_back(Vertex(-0.5f, -1.0f, 0.0f));

	//index buffer for triangle 1
	unsigned int TriangleIndecues[]		//might not need
	{
		0,1,2
	};


	vector<Vertex> FloorVerticies;
	FloorVerticies.push_back(Vertex(vec3(-1.0f, 1.0f, 0.0f), vec2(0, 0)));		//top left
	FloorVerticies.push_back(Vertex(vec3(1.0f, 1.0f, 0.0f), vec2(1, 0)));		//top right
	FloorVerticies.push_back(Vertex(vec3(1.0f, -1.0f, 0.0f), vec2(1, 1)));		//bottom right
	FloorVerticies.push_back(Vertex(vec3(-1.0f, -1.0f, 0.0f), vec2(0, 1)));	//bottom left

	//square 1
	vector<Vertex> SquareVerticies;
	SquareVerticies.push_back(Vertex(vec3(-1.0f, 1.0f, 0.0f), vec2(0, 0)));		//top left
	SquareVerticies.push_back(Vertex(vec3(100.0f, 1.0f, 0.0f), vec2(1, 0)));		//top right
	SquareVerticies.push_back(Vertex(vec3(100.0f, -1.0f, 0.0f), vec2(1, 1)));		//bottom right
	SquareVerticies.push_back(Vertex(vec3(-1.0f, -1.0f, 0.0f), vec2(0, 1)));	//bottom left

	//index buffer for square 1
	unsigned int SquareIndecues[]
	{
		0,1,2,0,2,3
	};

	unsigned int FloorIndecues[]
	{
		0,1,2,0,2,3
	};

	Mesh floor(&FloorVerticies[0], FloorVerticies.size(), &FloorIndecues[0], 6, true);
	//floor.m_trasform.setRotation(vec3(90, 0, 0));
	Mesh Triangle1(&TriangleVerticies[0], TriangleVerticies.size(), &TriangleIndecues[0], 3, true);
	Mesh Square1(&SquareVerticies[0], SquareVerticies.size(), &SquareIndecues[0], 6, true);

	vector<Vertex> MeshVertex = TriangleVerticies;	//change what verticies are used by assigning here

	GLuint VertexBufferObject = 0;
	glGenBuffers(1, &VertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float),
		MeshVertex.data(), GL_STATIC_DRAW);	//need to change MeshVertex so that the program works if changing the shape used

	GLuint VertexArrayObject = 0;
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);

	

	
	
	float index = 0;

	Input* input = new Input(*window, windowWidth, windowHeight);


	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	Camera  mainCamera(radians(90.0f), float(windowWidth)/float(windowHeight), nearPlane, farPlane, vec3(0, 0, -1), *input);

	Shader2D* basicShader = new Shader2D("../Graphics/Shaders/Resources/Basic", mainCamera);
	Shader* Shader3D = new Shader("../Graphics/Shaders/Resources/3D", mainCamera);
	Shader* depthShader = new Shader("../Graphics/Shaders/Resources/DepthShader", mainCamera);

	GLuint SQDiffuseTextureID = Texture2D::LoadTexture("../Graphics/assets/brickwall.jpg");
	GLuint SQNormalTextureID = Texture2D::LoadTexture("../Graphics/assets/brickwall_normal.jpg");

	vector<LightBase*> lightBaseVector;


	LightBase* light = new LightBase(vec3(0, -3, 1), vec3(1,1,1));	
	//light->setColour(vec3(0, 0, -1));
	lightBaseVector.push_back(light);


	string AmbiantLoc = "brick_wall_01_col.tga";
	string DiffuseLoc = "brick_wall_01_col.tga";
	string SpecLoc = "brick_wall_spec.tga";
	string NormalLoc = "brick_wall_03b_norm.tga";

	vector<uint> Indecies;

	vector<Vertex> LoadedVerts = OBJLoader::LoadOBJ("../Graphics/assets/Block", "blocks_01.obj",
		AmbiantLoc, DiffuseLoc, SpecLoc, NormalLoc, Indecies);

	GLuint AmbiantTextureID = Texture2D::LoadTexture("../Graphics/assets/Block/" + AmbiantLoc);
	GLuint DiffuseTextureID = Texture2D::LoadTexture("../Graphics/assets/Block/" + DiffuseLoc);
	GLuint SpeculerTextureID = Texture2D::LoadTexture("../Graphics/assets/Block/" + SpecLoc);
	GLuint NormalTextureID = Texture2D::LoadTexture("../Graphics/assets/Block/" + NormalLoc);

	Mesh cube(&LoadedVerts[0], LoadedVerts.size(), &Indecies[0], Indecies.size(), true);
	cube.m_trasform.setScale(vec3(0.05));
	cube.m_trasform.setPosition(vec3( 0, -1.5, -4));
	//cube.m_trasform.setRotation(vec3(radians(90.0f), 0, 0));

	floor.m_trasform.setScale(floor.m_trasform.getScale() * 50.0f);
	floor.m_trasform.setPosition(vec3(0, 1.0f,40));
	floor.m_trasform.setRotation(vec3(radians(-90.0f), 0, 0));
	
	Square1.m_trasform.setPosition(vec3(0.0f, 0.0f, 4.0f));

	/*LightBase* lightTwo = new LightBase(vec3(-0.5, -0.5, -1));
	lightTwo->m_Colour = vec3(0, 1, 0);
	lightBaseVector.push_back(lightTwo);

	LightBase* lightThree = new LightBase(vec3(0.5, -0.5, -1));
	lightThree->m_Colour = vec3(0, 0, 1);
	lightBaseVector.push_back(lightThree);

	LightBase* lightFour = new LightBase(vec3(-0.5, 0.5, -1));
	lightFour->m_Colour = vec3(1, 1, 1);
	lightBaseVector.push_back(lightFour);*/

	//int i = 0;

	//SDL_SetWindowGrab(window, SDL_TRUE);

	//snaps the mouse cursor to the centre of the screen then makes it invisable
	//if using mouse camera movements in must be snaped before the main update loop starts 
	SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);
	SDL_ShowCursor(0);

	glEnable(GL_CULL_FACE);

	//create depth texture
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	GLuint shadowMapID;
	int ShadowWidth = 2048, ShadowHeight = 2048;

	glGenTextures(1, &shadowMapID);
	glBindTexture(GL_TEXTURE_2D, shadowMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cerr << "ERROR Frame buffer is incomplete" << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	while (!input->KeyIsPressed(KEY_ESCAPE))
	{
		//
		//clear screen
		//
		glViewport(0, 0, ShadowWidth, ShadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		mat4 LSM = createLightSpaceMatrix(nearPlane, farPlane, lightBaseVector);

		glCullFace(GL_FRONT);
		depthShader->Bind();
		depthShader->UpdateForShadows(cube.m_trasform, LSM);
		
		cube.Draw();
		glCullFace(GL_BACK);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0.4f, 0.69f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_BACK);	//this doesnt need to be in the update loop // culls the back side of the plane

		//
		//update logic
		//
		
		/*
		light->setPosition(vec3(sin(i)*1, sin(i)*1, -1));								//this is just a bit of fun messing around with the colour and position
		light->setColour(vec3(abs(sin(i)), abs(sin(i + 1)), abs(sin(i + 2))));
		i++;
		*/

		input->Update();
		mainCamera.Update();

		//camera should query input NOT input querying camera .... better for multiple cameras?

		if (input->KeyIsPressed(KEY_W))
		{
			//move cam foward
			mainCamera.moveInLocalX(0.1f);
		}

		if (input->KeyIsPressed(KEY_A))
		{
			//move cam left
			mainCamera.moveInLocalZ(-0.1f);
		}

		if (input->KeyIsPressed(KEY_S))
		{
			//move cam back
			mainCamera.moveInLocalX(-0.1f);
		}

		if (input->KeyIsPressed(KEY_D))
		{
			//move cam right
			mainCamera.moveInLocalZ(0.1f);
		}

		if (input->KeyIsPressed(KEY_SPACE))
		{
			//move cam up
			mainCamera.moveInLocalY(0.1f);
		}

		if (input->KeyIsPressed(KEY_C))
		{
			//move cam down
			mainCamera.moveInLocalY(-0.1f);
		}

		if (input->KeyIsPressed(KEY_T))
		{
			input->ToggelLockToScreen();
		}

		if (input->KeyIsPressed(KEY_UP))
		{
			light->AddPosition(vec3(0, -0.1f, 0));
		}
		if (input->KeyIsPressed(KEY_DOWN))
		{
			light->AddPosition(vec3(0, 0.1f, 0));
		}
		if (input->KeyIsPressed(KEY_LEFT))
		{
			light->AddPosition(vec3(-0.1f,0,0));
		}
		if (input->KeyIsPressed(KEY_RIGHT))
		{
			light->AddPosition(vec3(0.1f,0 ,0));
		}

		light->draw(&mainCamera);
		//
		///draw
		//

		//Triangle1.m_trasform.setScale(vec3(1, 1, 1));
		//floor.m_trasform.setScale(vec3(1, 1, 1));

		

		basicShader->Bind();

		//basicShader->Update(Triangle1.m_trasform);

		glActiveTexture(GL_TEXTURE0);
		GLuint TextureLoc = glGetUniformLocation(basicShader->getProgram(), "texture_diffuse");
		glUniform1i(TextureLoc, 0); //location 0
		glBindTexture(GL_TEXTURE_2D, SQDiffuseTextureID);
		
		glActiveTexture(GL_TEXTURE1);
		TextureLoc = glGetUniformLocation(basicShader->getProgram(), "texture_normal");
		glUniform1i(TextureLoc, 1);	//location 1
		glBindTexture(GL_TEXTURE_2D, SQNormalTextureID);

		glActiveTexture(GL_TEXTURE2);
		TextureLoc = glGetUniformLocation(basicShader->getProgram(), "texture_shadow");
		glUniform1i(TextureLoc, 2);
		glBindTexture(GL_TEXTURE_2D, shadowMapID);
		
		basicShader->Update(floor.m_trasform, lightBaseVector, LSM);

		floor.Draw();

		basicShader->Update(Square1.m_trasform, lightBaseVector, LSM);

		Square1.Draw();






		//Shader3D->Bind();

		//glActiveTexture(GL_TEXTURE0);
		//TextureLoc = glGetUniformLocation(Shader3D->getProgram(), "tex_colour");
		//glUniform1i(TextureLoc, 0); //location 0
		//glBindTexture(GL_TEXTURE_2D, DiffuseTextureID);

		//glActiveTexture(GL_TEXTURE1);
		//TextureLoc = glGetUniformLocation(Shader3D->getProgram(), "tex_normal");
		//glUniform1i(TextureLoc, 1);	//location 1
		//glBindTexture(GL_TEXTURE_2D, NormalTextureID);

		//glActiveTexture(GL_TEXTURE2);															//dont think these bottom two are going anywhere
		//TextureLoc = glGetUniformLocation(Shader3D->getProgram(), "texture_ambiant");
		//glUniform1i(TextureLoc, 2); //location 2
		//glBindTexture(GL_TEXTURE_2D, AmbiantTextureID);

		//glActiveTexture(GL_TEXTURE3);
		//TextureLoc = glGetUniformLocation(Shader3D->getProgram(), "tex_shadow");
		//glUniform1i(TextureLoc, 3);
		//glBindTexture(GL_TEXTURE_2D, shadowMapID);

		/*glActiveTexture(GL_TEXTURE3);
		TextureLoc = glGetUniformLocation(Shader3D->getProgram(), "texture_Specular");
		glUniform1i(TextureLoc, 3);	//location 3
		glBindTexture(GL_TEXTURE_2D, SpeculerTextureID);*/


		/*Shader3D->Update(cube.m_trasform, lightBaseVector, LSM);*/

		basicShader->Bind();

		glActiveTexture(GL_TEXTURE0);
		TextureLoc = glGetUniformLocation(basicShader->getProgram(), "texture_diffuse");
		glUniform1i(TextureLoc, 0); //location 0
		glBindTexture(GL_TEXTURE_2D, DiffuseTextureID);

		glActiveTexture(GL_TEXTURE1);
		TextureLoc = glGetUniformLocation(basicShader->getProgram(), "texture_normal");
		glUniform1i(TextureLoc, 1);	//location 1
		glBindTexture(GL_TEXTURE_2D, NormalTextureID);


		glActiveTexture(GL_TEXTURE2);
		TextureLoc = glGetUniformLocation(basicShader->getProgram(), "texture_shadow");
		glUniform1i(TextureLoc, 2);
		glBindTexture(GL_TEXTURE_2D, shadowMapID);

		basicShader->Update(cube.m_trasform, lightBaseVector, LSM);
		cube.Draw();
		

		//Triangle1.Draw();
		
		

		//lightTwo->draw(&mainCamera);
		//lightThree->draw(&mainCamera);
		//lightFour->draw(&mainCamera);

		//
		//present
		//
		SDL_GL_SwapWindow(window);

		//
		//delay before next update 
		//

		SDL_Delay(16);
		//SDL_Delay(160);
	}

	delete  input;
	input = nullptr;

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();

	return 0;
}