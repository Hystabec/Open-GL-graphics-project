#pragma once
#include "Transforms.hpp"
#include "Input.hpp"
#include <glew.h>

class Camera
{
public:
	Transform m_transform;
	mat4 m_perspectiveProjection;

	Camera(float fov, float aspect, float zNear, float zFar, vec3 position, Input& input): m_input(input)
	{
		m_transform.setPosition(position);
		m_perspectiveProjection = perspective(fov, aspect, zNear, zFar);
		m_forward = normalize(vec3(0) - m_transform.getPosition());
		//m_input = input;
	}

	vec3 getFoward() { return m_forward; };
	vec3 getUp() { return m_up; };
	vec3 getRight() { return m_right; };

	vec3 getTransformPos() { return m_transform.getPosition(); };
	void setTransformPos(vec3 newPos) { m_transform.setPosition(newPos); };

	mat4 viewMatrix() 
	{
		m_right = -normalize(cross(vec3(0, 1, 0), m_forward));
		m_up = cross(m_forward, m_right);

		return lookAt(m_transform.getPosition(), vec3(m_transform.getPosition() + m_forward), m_up);
	};

	void Update()
	{
		xOffSet = m_input.getMouseDelta().y * MouseSensitivity;
		yOffSet = m_input.getMouseDelta().x * MouseSensitivity;

		vec3 temp = { m_transform.getRotation().x + xOffSet, m_transform.getRotation().y + yOffSet, m_transform.getRotation().z };
		m_transform.setRotation(temp);

		//adding a rotation lock
		if (m_transform.getRotation().x > 89.9f)
		{
			temp = { 89.0, m_transform.getRotation().y, m_transform.getRotation().z };
			m_transform.setRotation(temp);
		}
		if (m_transform.getRotation().x < -89.9f)
		{
			temp = { -89.0, m_transform.getRotation().y, m_transform.getRotation().z };
			m_transform.setRotation(temp);
		}

		if (m_transform.getRotation().y >= 360)
		{
			temp = { m_transform.getRotation().x, 0, m_transform.getRotation().z };
			m_transform.setRotation(temp);
		}
		if (m_transform.getRotation().y < 0)
		{
			temp = { m_transform.getRotation().x, 360, m_transform.getRotation().z };
			m_transform.setRotation(temp);
		}

		vec3 front;
		front.x = -cos(radians(-(m_transform.getRotation().y - 90))) * cos(radians(m_transform.getRotation().x));
		front.y = -sin(radians(m_transform.getRotation().x));
		front.z = sin(radians(-(m_transform.getRotation().y - 90))) * cos(radians(m_transform.getRotation().x));

		m_forward = normalize(front);
	};
	//when adding mouse movements, create a function that adds an offset to the m_forward based on the direction that the mouse was moved in ||vec3(1,0,0) for X and vec3(0,1,0) for Y||
	void moveInLocalX(float amountToMove) 
	{
		vec3 temp{ (m_transform.getPosition().x + (m_forward.x * amountToMove)), (m_transform.getPosition().y + (m_forward.y * amountToMove)), (m_transform.getPosition().z + (m_forward.z * amountToMove)) };
		m_transform.setPosition(temp);
	};
	void moveInLocalY(float amountToMove)
	{
		vec3 temp{ (m_transform.getPosition().x + (m_up.x * amountToMove)), (m_transform.getPosition().y + (m_up.y * amountToMove)), (m_transform.getPosition().z + (m_up.z * amountToMove)) };
		m_transform.setPosition(temp);
	};
	void moveInLocalZ(float amountToMove)
	{
		amountToMove *= -1;

		vec3 temp{ (m_transform.getPosition().x + (m_right.x * amountToMove)), (m_transform.getPosition().y + (m_right.y * amountToMove)), (m_transform.getPosition().z + (m_right.z * amountToMove)) };
		m_transform.setPosition(temp);
	};

private:
	void setFoward(vec3 newForward) { m_forward = newForward; };
	void setUp(vec3 newUp) { m_up = newUp; };
	void setRight(vec3 newRight) { m_right = newRight; };

	Input& m_input;

	vec3 m_forward;
	vec3 m_up;
	vec3 m_right;

	GLfloat MouseSensitivity = 0.5;
	GLfloat xOffSet = 0;
	GLfloat yOffSet = 0;
};