#pragma once

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TextureCoord;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
	glm::vec3 Normal;

	Vertex(float x = 0, float y = 0, float z = 0)
	{
		this->Position.x = x;
		this->Position.y = y;
		this->Position.z = z;

		TextureCoord = { 0,0 };
		Normal = glm::vec3(0);

		Tangent = glm::vec3(0);
		BiTangent = glm::vec3(0);
	}

	Vertex(const glm::vec3 position) : Vertex(position.x, position.y, position.z)
	{

	}

	Vertex(const glm::vec3 position, const glm::vec2 texCoord)
	{
		this->Position = position;
		this->TextureCoord = texCoord;
		Normal = glm::vec3(0);

		Tangent = glm::vec3(0);
		BiTangent = glm::vec3(0);
	}

	Vertex(const glm::vec3 position, const glm::vec2 texCoord, const glm::vec3 normal)
	{
		this->Position = position;
		this->TextureCoord = texCoord;
		this->Normal = normal;

		Tangent = glm::vec3(0);
		BiTangent = glm::vec3(0);
	}

	Vertex(const Vertex& vert)
	{
		this->Position.x = vert.Position.x;
		this->Position.y = vert.Position.y;
		this->Position.z = vert.Position.z;

		TextureCoord = { vert.TextureCoord.x, vert.TextureCoord.y };

		this->Normal.x = vert.Normal.x;
		this->Normal.y = vert.Normal.y;
		this->Normal.z = vert.Normal.z;

		Tangent = glm::vec3(0);
		BiTangent = glm::vec3(0);
	}
};