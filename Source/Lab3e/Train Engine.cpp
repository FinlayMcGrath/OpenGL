#include "Train Engine.h"

TrainEngine::TrainEngine() : chimney(20, 5), engineTop(20, 5) 
{
	wheels[0] = Cylinder(20, 5);
	wheels[1] = Cylinder(20, 5);
}

void TrainEngine::draw()
{
	glPushMatrix();
		glScalef(1.2f, 0.05f, 0.05f);
		glRotatef(-90, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, -0.1f / 1.2, 0.0f);
		wheels[0].draw();
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.66f / 0.05f);
			wheels[1].draw();
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
		glScalef(0.55f, 0.55f, 1.25f);
		glRotatef(90, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.50f / 0.55f, -0.25f / 1.25f, -0.50f / 0.55f);
		engineTop.draw();
	glPopMatrix();
	glPushMatrix();
		glScalef(0.1f, 0.25f, 0.1f);
		glTranslatef(0.5f / 0.1f, 1.0f / 0.25, 0.0f);
		chimney.draw();
	glPopMatrix();
}

void TrainEngine::init()
{

}