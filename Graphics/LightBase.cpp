#include "LightBase.h"
#include "Camera.hpp"
#include <glew.h>

LightBase::LightBase(vec3 newTransform, vec3 newColour)
{
	m_Colour = newColour;
	m_transform = newTransform;
}

LightBase::~LightBase()
{

}

void LightBase::draw(Camera* cam)
{
	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&cam->m_perspectiveProjection);
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 MV = cam->viewMatrix() * m_transform.GetModel();
	glLoadMatrixf((const GLfloat*)&MV[0]);

	glBegin(GL_LINES);
		glm::vec3 p1 = this->m_transform.getPosition();
		glm::vec3 p2 = p1;

		glColor3f(1, 0, 0);
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(1, 0, 0) * 0.1f;
		glColor3f(1, 0, 0);
		glVertex3fv(&p2.x);

		glColor3f(0, 1, 0);
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(0, 1, 0) * 0.1f;
		glColor3f(0, 1, 0);
		glVertex3fv(&p2.x);

		glColor3f(0, 0, 1);
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(0, 0, 1) * 0.1f;
		glColor3f(0, 0, 1);
		glVertex3fv(&p2.x);
	glEnd();
}

void LightBase::setPosition(vec3 newPos)
{
	m_transform.setPosition(newPos);
}

Transform LightBase::getTransform()
{
	return m_transform;
}

void LightBase::setColour(vec3 newColour)
{
	m_Colour = newColour;
}

vec3 LightBase::getColour()
{
	return m_Colour;
}

void LightBase::AddPosition(vec3 pos)
{

	m_transform.setPosition(m_transform.getPosition() + pos);
}
