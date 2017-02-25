#include "Train Carriage.h"

TrainCarriage::TrainCarriage() : link(20, 5)
{
	wheels[0] = Cylinder(20, 5);
	wheels[1] = Cylinder(20, 5);
}

void TrainCarriage::draw()
{
	//draws out the shapes that make up a train carriage
	glPushMatrix();
		glScalef(1.2f, 0.05f, 0.05f);
		glRotatef(-90, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, -0.1f / 1.2, 0.33f / 0.05f);
		wheels[0].draw();
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.33f / 0.05f);
			wheels[1].draw();
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
		glScalef(0.05f, 0.05f, 0.5f);
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.5f / 0.05f, 0.0f, 1.2f);
		link.draw();
	glPopMatrix();
	glPushMatrix();
		glScalef(0.1f, 0.25f, 0.1f);
		glTranslatef(0.0f, 0.5f / 0.25f, 0.0f);
		glPushMatrix();
			struts[0].draw();
			glTranslatef(0.0f, 0.0f, 0.9f / 0.1f);
			struts[1].draw();
		glPopMatrix();
		glPushMatrix();			
			glTranslatef(0.9f / 0.1f, 0.0f, 0.0f);
			struts[2].draw();
			glTranslatef(0.0f, 0.0f, 0.9f / 0.1f);
			struts[3].draw();
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
		glScalef(1.0f, 0.5f, 1.0f);
		base.draw();
	glPopMatrix();
	glPushMatrix();
		glScalef(1.0f, 0.25f, 1.0f);
		glTranslatef(0.0f, 0.75f / 0.25f, 0.0f);
		roof.draw();
	glPopMatrix();
}

void TrainCarriage::init()
{
	//initialise opengl for cubes
	base.init();
	roof.init();
	for (int i = 0; i < 4; i++)
	{
		struts[i].init();
	}
}