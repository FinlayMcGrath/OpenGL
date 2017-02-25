#include "Fighter.h"

Fighter::Fighter()
{	
	velocity = 25;
	rotationalVelocity = 72;
	w = 1;
	setScale(0.05f, 0.05f, 0.05f);	
	update();
}

void Fighter::update()
{
	float cosR, cosP, cosY;	//temp values for sin/cos
	float sinR, sinP, sinY;	
	
	cosY = cosf(rotation.getY() * PI / 180.0f);
	cosP = cosf(rotation.getX() * PI / 180.0f);
	cosR = cosf(rotation.getZ() * PI / 180.0f);
	sinY = sinf(rotation.getY() * PI / 180.0f);
	sinP = sinf(rotation.getX() * PI / 180.0f);
	sinR = sinf(rotation.getZ() * PI / 180.0f);
	
	//This using the parametric equation of a sphere

	forward.setX(sinY * cosP);
	forward.setY(sinP);
	forward.setZ(cosP * -cosY);

	// Up Vector
	up.setX(-cosY * sinR - sinY * sinP * cosR);
	up.setY(cosP * cosR);
	up.setZ(sinY * sinR - sinP * cosR * -cosY);

	// Side Vector (right)
	// this is a cross product between the forward and up vector. 
	// If you don’t need to calculate this,  don’t do it. 
	Vec3 temp = forward.cross(up);
	right.set(temp.getX(), temp.getY(), temp.getZ());

	camera.setRotation(rotation.getX(), rotation.getY(), rotation.getZ());
	camera.setPosition(position.getX(), position.getY(), position.getZ());
	camera.update();
	camera.setPosition(position.getX() + (camera.getUp().getX() * 10) - (camera.getForward().getX() * 20), 
						position.getY() + (camera.getUp().getY() * 10) - (camera.getForward().getY() * 20),
						position.getZ() + (camera.getUp().getZ() * 10) - (camera.getForward().getZ() * 20));

	//calculates a quaternion to allow correct rotation without gimble locks. Sadly does not work properly
	float p = rotation.getZ() * (PI / 180) / 2.0f;
	float y = rotation.getY() * (PI / 180) / 2.0f;
	float r = rotation.getX() * (PI / 180) / 2.0f;
 
	//Components
    float C1 = cos(y);
    float C2 = cos(p);
    float C3 = cos(r);
    float S1 = sin(y);
    float S2 = sin(p);
    float S3 = sin(r);
 
	//Create the final values
    w = ((C1 * C2 * C3) - (S1 * S2 * S3));
    x = (S1 * S2 * C3) + (C1 * C2 * S3);
    y = (S1 * C2 * C3) + (C1 * S2 * S3);
    z = (C1 * S2 * C3) - (S1 * C2 * S3);

	//turns the quaternion values into a matrix that describes the rotation to be applied
	//Column 1
    rotationMatrix[0] = (w*w) + (x*x) - (y*y) - (z*z);
    rotationMatrix[1] = (2*x*y) + (2*w*z);
    rotationMatrix[2] = (2*x*z) - (2*w*y);
    rotationMatrix[3] = 0;
    //Column 2
    rotationMatrix[4] = (2*x*y) - (2*w*z);
    rotationMatrix[5] = (w*w) - (x*x) + (y*y) - (z*z);
    rotationMatrix[6] = (2*y*z) + (2*w*x);
    rotationMatrix[7] = 0;
    //Column 3
    rotationMatrix[8] = (2*x*z) + (2*w*y);
    rotationMatrix[9] = (2*y*z) - (2*w*x);
    rotationMatrix[10] = (w*w) - (x*x) - (y*y) + (z*z);
    rotationMatrix[11] = 0;
    //Column 4
    rotationMatrix[12] = 0;
    rotationMatrix[13] = 0;
    rotationMatrix[14] = 0;
    rotationMatrix[15] = 1;
}

void Fighter::draw()
{	
	glPushMatrix();		
		glTranslatef(position.getX() + up.getX() - forward.getX(), position.getY() + up.getY() - forward.getY(), position.getZ() + up.getZ() - forward.getZ());
		glMultMatrixf((GLfloat *) rotationMatrix);
		glScalef(scale.getX(), scale.getY(), scale.getZ());
		render();
	glPopMatrix();
}

Bullet Fighter::fire()
{
	//instantiate a bullet based upon the fighter
	Bullet bullet(position, rotation, forward, velocity);
	return bullet;
}