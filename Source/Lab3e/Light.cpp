#include "Light.h"

void Light::init(GLenum lightID_, GLfloat ambient_[], GLfloat diffuse_[], GLfloat position_[], GLfloat specular_[], 
				GLfloat constant, GLfloat linear, GLfloat quadratic)
{
	//takes in light values and stores them
	lightID = lightID_;

	for (int i = 0; i < 4; i++)
	{
		ambient[i] = ambient_[i];
		diffuse[i] = diffuse[i];
		specular[i] = specular_[i];
		position[i] = position_[i];
	}

	glLightf(lightID, GL_CONSTANT_ATTENUATION, constant);
	glLightf(lightID, GL_LINEAR_ATTENUATION, linear);
	glLightf(lightID, GL_QUADRATIC_ATTENUATION, quadratic);

	glLightfv(lightID, GL_AMBIENT,  ambient);
	glLightfv(lightID, GL_DIFFUSE,  diffuse);
	glLightfv(lightID, GL_POSITION, position);
	glLightfv(lightID, GL_SPECULAR, specular);
	glEnable(lightID);
}

void Light::setPosition(GLfloat pos[])
{
	for (int i = 0; i < 4; i++)
	{		
		position[i] = pos[i];
	}
}