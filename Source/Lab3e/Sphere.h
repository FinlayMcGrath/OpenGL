#ifndef _SPHERECLASS_H_
#define	_SPHERECLASS_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere(int longtitude, int latitude);
	void init(){};
	void draw();
	void update(){};
private:
	int longitude, latitude;
};

#endif