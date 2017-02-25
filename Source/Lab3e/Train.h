#ifndef _TRAIN_H_
#define	_TRAIN_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Train Carriage.h"
#include "Train Engine.h"
#include "Vec3.h"
#include "Camera.h"
#include "Light.h"
#include <math.h>
#include "GameObject.h"
 
#define numCarriages 8

class Train : public GameObject
{
public:
	Train(float x = 0, float y = 0, float z = 0);
	void init();
	void draw();
	void update();
	Camera* getCamera() {return &camera;};
	Light getLight() {return light;};
private:
	void updateCamera();
	TrainEngine engine;
	TrainCarriage carriage[8];
	Light light;
	Camera camera;
};

#endif