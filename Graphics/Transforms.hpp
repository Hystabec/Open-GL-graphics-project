#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

class Transform
{
public:
	Transform(const vec3& pos = vec3(), const vec3& rot = vec3(), const vec3& scale = vec3(1,1,1) ):
		m_position(pos),
		m_rotation(rot),
		m_scale(scale)
	{}

	~Transform(){}

	inline mat4 GetModel() const
	{
		mat4 posMatrix = translate(m_position);
		mat4 rotXMatrix = rotate(m_rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(m_rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(m_rotation.z, vec3(0, 0, 1));
		mat4 scaleMatix = scale(m_scale);

		mat4 rotMatix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * rotMatix * scaleMatix;
	}

	vec3 getPosition() { return m_position; };
	vec3 getRotation() { return m_rotation; };
	vec3 getScale() { return m_scale; };

	void setPosition(vec3 pos) { m_position = pos; };
	void setRotation(vec3 rot) { m_rotation = rot; };
	void setScale(vec3 scale) { m_scale = scale; };

private:
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;;
};

#endif // !TRANSFORM_H