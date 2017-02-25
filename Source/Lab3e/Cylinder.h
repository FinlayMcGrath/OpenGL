#ifndef CYLINDER_H
#define CYLINDER_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Shape.h"
#include "disc.h"

class Cylinder : public Shape
{	
public:
	Cylinder() {}
	void init(){};
	Cylinder(int noEdgePoints, int height_, const bool hasBottom_ = true, const bool hasTop_ = true);
	void draw();
	void update(){};
private:
	Disc bottom, top;
	bool hasBottom, hasTop;
	int edgePoints, height;
};

#endif