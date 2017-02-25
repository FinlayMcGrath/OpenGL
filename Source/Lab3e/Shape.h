#ifndef _SHAPE_H
#define _SHAPE_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "Vec3.h"
#include "GameObject.h"

#define PI 3.14159

class Shape : public GameObject
{
public:
	virtual void init() = 0;
	virtual void update() = 0;
	void init(Vec3 position_, Vec3 rotation_, Vec3 scale_);
	void render(int vertices);
protected:
	void render1();
	void render3();
	float* verts;
	float* norms;
	float* texCoords;
	GLubyte* indices;
	int vertices;
};
#endif 
