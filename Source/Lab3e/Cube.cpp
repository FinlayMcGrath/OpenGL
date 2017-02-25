#include "Cube.h"

Cube::Cube()
{		
	scale.set(1.0f, 1.0f, 1.0f);
}

void Cube::draw()
{
	glCallList(dList);
}

void Cube::init()
{
	GLuint qList;

	qList = glGenLists(1);
	
	//create a list to store a square
	glNewList(qList, GL_COMPILE);
		glBegin (GL_TRIANGLES);
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);

			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);

			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
		
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);                        
		
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
		glEnd();
	glEndList();

	dList = glGenLists(1);

	//create a list to store the cube and use the square list that has been manipulated to form a cube
	glNewList(dList, GL_COMPILE);
		glPushMatrix();
			for (int i = 0; i < 4; i++)
			{
				glCallList(qList);
				glRotatef(90, 0.0f, 1.0f, 0.0f);
				glTranslatef(-1.0f, 0.0f, 0.0f);
			}
		glPopMatrix();	
		glPushMatrix();	
			glRotatef(90, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, -1.0f);
			glCallList(qList);
		glPopMatrix();
		glPushMatrix();
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -1.0f, 0.0f);	
			glCallList(qList);
		glPopMatrix();
	glEndList();
}