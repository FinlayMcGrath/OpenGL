#include "disc.h"

Disc::Disc(int vertice)
{
	vertices = vertice+1;
	verts = new float[vertices * 3];
	norms = new float[vertices * 3];
	texCoords = new float[vertices * 2];
	indices = new GLubyte[vertices + 1];

	float interval = (2 * PI) / (vertices-1);
	float theta = 0;
	verts[0] = 0;
	verts[1] = 0;
	verts[2] = 0;
	
	//for each vertex
	for (int i = 3; i < vertices * 3; i++)
	{
		//set position and norms
		verts[i] = cos(theta);
		norms[i] = 0;
		verts[++i] = 0;	
		norms[i] = 0;
		verts[++i] = sin(theta);
		norms[i] = 1;
		theta += interval; 
	}

	texCoords[0] = 0.5;
	texCoords[1] = 0.5;
	theta = 0;
	for (int i = 2; i < vertices * 2; i++)
	{		
		//set tex coords based upon position
		texCoords[i] = (cos(theta) / 2.0f) + 0.5;
		texCoords[++i] = (sin(theta) / 2.0f) + 0.5;
		theta += interval; 
	}

	for (int i = 0; i < vertices; i++)
	{
		indices[i] = i;
	}
	indices[vertices] = 1;
}

void Disc::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);

	glVertexPointer (3, GL_FLOAT, 0, verts);
	glNormalPointer (GL_FLOAT, 0, norms);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glIndexPointer(GL_TRIANGLE_FAN, 0, indices);

	glDrawElements(GL_TRIANGLE_FAN, vertices + 1, GL_UNSIGNED_BYTE, indices);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
}