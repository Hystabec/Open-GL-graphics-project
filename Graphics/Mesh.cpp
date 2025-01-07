#include "Mesh.h"
#include <vector>
#include "Vertex.h"
#include <glm/gtx/normal.hpp>

Mesh::Mesh(Vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices, bool CalcNormals)
{
	CalculateTangentBiTangent(verts, vertCount, indices, numIndices);
	m_drawCount = numIndices;

	std::vector<vec3> position;
	std::vector<vec2> texCoords;
	std::vector<vec3> Tangents;
	std::vector<vec3> BiTangents;
	std::vector<vec3> Normals;
	

	for (unsigned int i = 0; i < vertCount; i++)
	{
		position.push_back(verts[i].Position);
		texCoords.push_back(verts[i].TextureCoord);
		Tangents.push_back(verts[i].Tangent);
		BiTangents.push_back(verts[i].BiTangent);
		if (!CalcNormals)
			Normals.push_back(verts[i].Normal);
	}

	
	if (CalcNormals)
	{	
		for (unsigned int i = 0; i < numIndices; i += 3)
		{
			Normals.resize(vertCount);
			vec3 Vert1 = position[indices[i]];
			vec3 Vert2 = position[indices[i + 1]];
			vec3 Vert3 = position[indices[i + 2]];

			vec3 normal = triangleNormal(Vert1, Vert2, Vert3);

			//normal = vec3(1, 0, 0);

			Normals[indices[i]] += normal;
			Normals[indices[i + 1]] += normal;
			Normals[indices[i + 2]] += normal;
		}
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, m_vertexBufferObjects);

	//position
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount *sizeof(position[0]),
		&position[0], GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(POSITION_VB);

	//texture coords
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(texCoords[0]),
		&texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(TEXCOORD_VB, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TEXCOORD_VB);

	

	//Normal
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[Normal_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Normals[0]),
		&Normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(Normal_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Normal_VB);

	//Tangent
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[Tangent_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Tangents[0]),
		&Tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(Tangent_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Tangent_VB);

	//BiTangent
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[BiTangent_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(BiTangents[0]),
		&BiTangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(BiTangent_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(BiTangent_VB);

	//index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferObjects[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int),
		indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::CalculateTangentBiTangent(Vertex* verticies, unsigned int vertCount, unsigned int* indicies, unsigned int numIndices)
{
	for (unsigned int i = 0; i < numIndices; i += 3)
	{
		//grab the vertex for the current indece
		Vertex v0 = verticies[indicies[i]];
		Vertex v1 = verticies[indicies[i + 1]];
		Vertex v2 = verticies[indicies[i + 2]];

		vec3 edge1 = v1.Position - v0.Position;
		vec3 edge2 = v2.Position - v0.Position;

		GLfloat deltaU1 = v1.TextureCoord.x - v0.TextureCoord.x;
		GLfloat deltaV1 = v1.TextureCoord.y - v0.TextureCoord.y;
		GLfloat deltaU2 = v2.TextureCoord.x - v0.TextureCoord.x;
		GLfloat deltaV2 = v2.TextureCoord.y - v0.TextureCoord.y;

		GLfloat f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

		vec3 tangent;
		vec3 BiTangent;

		tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
		tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
		tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

		BiTangent.x = f * (-deltaU2 * edge1.x + deltaU1 * edge2.x);
		BiTangent.y = f * (-deltaU2 * edge1.y + deltaU1 * edge2.y);
		BiTangent.z = f * (-deltaU2 * edge1.z + deltaU1 * edge2.z);

		v0.Tangent += tangent;
		v1.Tangent += tangent;
		v2.Tangent += tangent;

		v0.BiTangent += BiTangent;
		v1.BiTangent += BiTangent;
		v2.BiTangent += BiTangent;

		verticies[indicies[i]] = v0;
		verticies[indicies[i+1]] = v1;
		verticies[indicies[i+2]] = v2;
	}

	for (unsigned int i = 0; i < vertCount; i++)
	{
		verticies[i].Tangent = normalize(verticies[i].Tangent);
		verticies[i].BiTangent = normalize(verticies[i].BiTangent);
	}
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}