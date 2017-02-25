#ifndef _TRAINENGINE_H_
#define	_TRAINENGINE_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "GameObject.h"
#include "Cylinder.h"
#include "Cube.h"

class TrainEngine : public GameObject
{
public:
	TrainEngine();
	void update(){};
	void init();
	void draw();
private:
	Cylinder wheels[2];
	Cylinder chimney, engineTop;
};

#endif