#include "Bullet.h"

Bullet::Bullet(Vec3 position_, Vec3 rotation_, Vec3 forward_, float velocity_)
{
	position = position_;
	rotation = rotation_;
	forward = forward_;
	velocity = velocity_ ;
	scale.set(1.0f, 1.0f, 1.0f);
	init();
}

void Bullet::update(float dt)
{
	//keep the bullet moving forward
	position.set(position.getX() + (velocity * dt * forward.getX()), position.getY() - (velocity * dt * forward.getY()), position.getZ() + (velocity * dt * forward.getZ())); 
}