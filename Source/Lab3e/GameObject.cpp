#include "GameObject.h"

GameObject::GameObject()
{

}

void GameObject::setScale(float x, float y, float z)
{
	scale.setX(x);
	scale.setY(y);
	scale.setZ(z);
}

void GameObject::setPosition(float x, float y, float z)
{
	position.setX(x);
	position.setY(y);
	position.setZ(z);
}

void GameObject::setRotation(float x, float y, float z)
{
	rotation.setX(x);
	rotation.setY(y);
	rotation.setZ(z);
}
