#include "Train.h"

Train::Train(float x, float y, float z)
{
	//give variables with initial values
	position.set(x, 0.1f, z);
	scale.set(3.0f, 3.0f, 3.0f);
	rotation.set(0.0f, 90.0f, 0.0f);

	//set each carriage to be a little bit further along the Z axis than the previous one
	float pos = 1.5f;
	for (int i = 0; i < numCarriages; i++)
	{
		carriage[i].setRotation(rotation.getX(), rotation.getY(), rotation.getZ());
		carriage[i].setPosition(0.0f, 0.0f, pos);
		pos += 1.5f;
	}
	velocity = 10;
	rotationalVelocity = 50;	
	camera.setPosition(position.getX(), position.getY() + 5, position.getZ() + 5);
	camera.setRotation(-45, rotation.getY(), 0);	
	update();
}

void Train::draw()
{	
	//draw engine based upon train's position, rotation and scale
	glPushMatrix();		
		glTranslatef(position.getX(), position.getY(), position.getZ());
		glRotatef(rotation.getY(), 0, -1, 0);
		glScalef(scale.getX(), scale.getY(), scale.getZ());
		engine.draw();
	glPopMatrix();

	Vec3 rot = rotation;
	Vec3 tempRot;
	//draw each carriage, using the previous rotation value to simulate movement
	for (int i = 0; i < numCarriages; i++)
	{
		glPushMatrix();
			//translate back in line with rest of train
			glTranslatef(position.getX(), 0.0f, position.getZ());
			//rotate each carriage based upon the previous train part, causes a delay
			glRotatef(rot.getY(), 0, -1, 0);
			glScalef(scale.getX(), scale.getY(), scale.getZ());
			glTranslatef(carriage[i].getPosition().getX(), 0.0f, carriage[i].getPosition().getZ());
			tempRot = carriage[i].getRotation();
			carriage[i].setRotation(rot.getX(), rot.getY(), rot.getZ());
			rot = tempRot;
			carriage[i].draw();
		glPopMatrix();
	}
}

void Train::init()
{
	//initialise the cubes for opengl
	engine.init();
	for (int i = 0; i < numCarriages; i++)
	{
		carriage[i].init();
	}
	//initialise the light
	GLfloat Light_Ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat Light_Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat Light_Position[] = {position.getX() - forward.getX(), position.getY() + 5.0f, position.getZ() - forward.getZ(), 1.0};
	GLfloat Light_Specular[] = {1.0, 1.0, 1.0, 1.0};
	light.init(GL_LIGHT1, Light_Ambient, Light_Diffuse, Light_Position, Light_Specular, 1.0f, 0.125f, 0.0f);
	glEnable(GL_LIGHT1);
}

void Train::updateCamera()
{
	//update camera based upon the train's position
	camera.setRotation(-45, rotation.getY(), 0.0f);
	camera.setPosition(position.getX(), (position.getY()), position.getZ());
	camera.update();
	camera.setPosition(position.getX() + camera.getUp().getX() - (camera.getForward().getX() * 60), 
						position.getY() + (camera.getUp().getY() * 5) - camera.getForward().getY(),
						position.getZ() + camera.getUp().getZ() - (camera.getForward().getZ() * 60));
}

void Train::update()
{
	//calculate the camera's direction in order to ensure the light follows the train
	float cosP, cosY;	
	float sinP, sinY;	
	
	cosY = cosf(rotation.getY() * (PI / 180.0f));
	cosP = cosf(rotation.getX() * (PI / 180.0f));
	sinY = sinf(rotation.getY() * (PI / 180.0f));
	sinP = sinf(rotation.getX() * (PI / 180.0f));

	forward.setX(sinY * cosP);
	forward.setY(sinP);
	forward.setZ(cosP * -cosY);
	GLfloat pos[] = {position.getX() - forward.getX(), position.getY() + 5.0f, position.getZ() - forward.getZ(), 1.0};
	light.setPosition(pos);
	updateCamera();
}