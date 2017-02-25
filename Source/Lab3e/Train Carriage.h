#ifndef _TRAINCARRIAGE_H_
#define	_TRAINCARRIAGE_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Cylinder.h"
#include "Cube.h"

class TrainCarriage : public GameObject
{	
public:
	TrainCarriage();
	void init();
	void draw();
	void update(){};
private:
	Cylinder wheels[2], link;
	Cube base, roof, struts[4];
};

#endif