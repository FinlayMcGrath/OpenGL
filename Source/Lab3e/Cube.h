#ifndef _CUBE_H_
#define	_CUBE_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube();
	void init();
	void draw();
	void update(){};
private:
	GLuint dList;
};

#endif