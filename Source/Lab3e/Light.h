#ifndef LIGHT_H
#define LIGHT_H

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

//class to store data for a light
class Light
{
public:
	Light() {};
	void init(GLenum lightID_, GLfloat ambient_[], GLfloat diffuse_[], GLfloat position_[], GLfloat specular_[], 
			GLfloat constant, GLfloat linear, GLfloat quadratic);
	GLenum getLightID(){return lightID;};
	GLfloat* getAmbient(){return ambient;};
	GLfloat* getDiffuse(){return diffuse;};
	GLfloat* getPosition(){return position;};
	void setPosition(GLfloat pos[]);
private:
	GLenum lightID;	
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[4];
};

#endif