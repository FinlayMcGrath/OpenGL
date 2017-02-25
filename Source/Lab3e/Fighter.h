#ifndef _FIGHTER_H_
#define _FIGHTER_H_

#include "Model.h"
#include "Bullet.h"

class Fighter : public Model, public GameObject
{
public:
	Fighter();
	void draw();
	void update();
	Bullet fire();
};

#endif