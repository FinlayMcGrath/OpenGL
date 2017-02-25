#ifndef _BULLET_H_
#define	_BULLET_H_

#include "GameObject.h"
#include "Cube.h"

class Bullet : public Cube
{
public:
	Bullet(Vec3 position_, Vec3 rotation_, Vec3 forward_, float velocity_);
	void update(float dt);
};

#endif