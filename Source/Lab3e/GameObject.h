#ifndef _GAMEOBJECT_H_
#define	_GAMEOBJECT_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Vec3.h"
#include "Camera.h"

//base gameobject class
class GameObject
{
public:
	GameObject();
	~GameObject(){};
	virtual void update() = 0;
	virtual void draw() = 0;

	void setPosition(float x, float y, float z);
	Vec3 getPosition() {return position;};
	void setRotation(float x, float y, float z);
	Vec3 getRotation() {return rotation;};	
	void setScale(float x, float y, float z);
	Vec3 getScale() {return scale;};
	float getRotationalVelocity(){return rotationalVelocity;};
	void setRotationalVelocity(float RotationalVelocity){rotationalVelocity = RotationalVelocity;};
	Vec3 getRight() {return right;};
	Vec3 getForward() {return forward;};
	float getVelocity(){return velocity;};
	void setVelocity(float Velocity){velocity = Velocity;};	
	Vec3 getUp() {return up;};
	void setUp(float x, float y, float z);
	Camera* getCamera() {return &camera;};

protected:	
	Camera camera;
	Vec3 rotation, position, scale, forward, up, right;
	float velocity, rotationalVelocity, rotationMatrix[16];
	double x, y, z, w;
};

#endif