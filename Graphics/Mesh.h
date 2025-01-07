#pragma once
#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glew.h>
#include "Transforms.hpp"
class Vertex;


class Mesh
{
public:
	Mesh(Vertex* verts, unsigned int vertCount, unsigned int *indices, unsigned int numIndices, bool CalcNormals = false);
	void Draw();
	void CalculateTangentBiTangent(Vertex* verticies, unsigned int vertCount, unsigned int* indicies, unsigned int numIndices);
	~Mesh();
	Transform m_trasform;

private:
	unsigned int m_drawCount;

	enum   //made to avoid magic numbers
	{
		POSITION_VB,	//0
		TEXCOORD_VB,	//1
		Normal_VB,		//2
		Tangent_VB,		//3
		BiTangent_VB,	//4
		INDEX_VB,		//5
		NUM_BUFFERS		//6
	};

	GLuint m_vertexBufferObjects[NUM_BUFFERS];
	GLuint m_vertexArrayObject = 0;
};

#endif // !MESH_H
