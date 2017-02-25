#ifndef _DISCCLASS_H_
#define	_DISCCLASS_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Shape.h"

class Disc : public Shape
{
public:
	Disc() {}
	Disc(int vertice);
	void init(){};
	void draw();
	void update(){};
};

#endif