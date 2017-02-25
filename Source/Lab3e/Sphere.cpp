#include "Sphere.h"

Sphere::Sphere(int x, int y)
{
	longitude = x;
	latitude = y;
	vertices = (longitude  + 1) * (latitude + 1);
	float thetaInterval = (2*PI) / longitude;
	float deltaInterval = PI / latitude;
	float theta = 0;
	float delta = 0;
	
	verts = new float[vertices * 3];
	norms = new float[vertices * 3];
	texCoords = new float[vertices * 2];
	indices = new GLubyte[vertices * 4];

	int counter = 0;
	for (int i = 0; i <= longitude; i++)
	{
		for (int j = 0; j <= latitude; j++)
		{
			verts[counter] = cos(theta) * sin(delta);
			norms[counter] = 0;
			verts[++counter] = cos(delta);	
			norms[counter] = 0;
			verts[++counter] = sin(theta) * sin(delta);
			norms[counter] = 1;
			counter++;
			theta += thetaInterval;

		}		
		delta += deltaInterval;
	}

	theta = 0;
	delta = 0;
	counter = 0;
	for (int i = 0; i <= longitude; i++)
	{
		for (int j = 0; j <= latitude; j++)
		{
			texCoords[counter] = (cos(theta) + sin(delta) / 2);
			texCoords[++counter] = (cos(delta) / 2);
			counter++;
			theta += thetaInterval;
		}		
		delta += deltaInterval;
	}
	

	counter = 0;
	/*while (counter <= vertices*4)
	{		
		indices[counter  ] = counter;
		indices[counter+1] = counter + longitude; 
		indices[counter+2] = counter + 1; 
		indices[counter+3] = counter + longitude + 1; 
		counter += 4;
	}*/

	for (int j = 0; j <= longitude; j++)
	{
		for (int i = 0; i <= latitude; i++)
		{
			int vertOffset = j*longitude + i;
			int quadOffset = vertOffset * 4 + vertOffset;

			indices[quadOffset+0] = vertOffset;
			indices[quadOffset+1] = vertOffset + longitude;
			indices[quadOffset+2] = vertOffset + longitude + 1;
			indices[quadOffset+3] = vertOffset + 1;
		}
	}
}

void Sphere::draw()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);

	glVertexPointer (4, GL_FLOAT, 0, verts);
	glNormalPointer (GL_FLOAT, 0, norms);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	//glIndexPointer(GL_QUADS, 0, indices);

	//glDrawElements(GL_QUADS, vertices*4, GL_UNSIGNED_BYTE, indices);
	glDrawArrays(GL_QUADS, 0, vertices);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
}