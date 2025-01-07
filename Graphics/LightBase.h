#pragma once
#include "Transforms.hpp"

class Camera;

class LightBase
{
public:
	LightBase(vec3 newTransform, vec3 newColour);
	~LightBase();
	void draw(Camera* cam);
	void setPosition(vec3 newPos);
	Transform getTransform();
	void setColour(vec3 newColour);
	vec3 getColour();
	
	void AddPosition(vec3 pos);

private:
	Transform m_transform;
	vec3 m_Colour;
};