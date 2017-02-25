#include "Cylinder.h"

Cylinder::Cylinder(int noEdgePoints, int height_, const bool hasBottom_, const bool hasTop_)
{
	height = height_;
	hasBottom = hasBottom_;
	hasTop = hasTop_;
	edgePoints = noEdgePoints;
	vertices = edgePoints * height * 4;

	verts = new float[vertices * 3];
	norms = new float[vertices * 3];
	texCoords = new float[vertices * 2];

	float interval = (2 * PI) / (float)edgePoints;
	float theta = 0;
	int vertPos = 0;

	int iVertexPos = 0;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < edgePoints; i++)
		{
			//for each point on a quad
			for (int v = 0; v < 4; v++)
			{
				//if the third point
				if (v == 2)
				{
					//rotate around the cylinder to the next point
					theta += interval;
				}

				//set x position 
				verts[vertPos] = cos(theta);
				norms[vertPos] = verts[vertPos];

				vertPos++;
				//set the y position to be in relation to the height
				if ((v == 1) || (v == 2))
				{
					verts[vertPos] = j / height;
				}
				else
				{
					verts[vertPos] = (j + 1) / height;
				}
				norms[vertPos] = verts[vertPos];

				vertPos++;
				//set z position
				verts[vertPos] = sin(theta);
				norms[vertPos] = verts[vertPos];

				vertPos++;
			}
		}
		theta = 0;
	}

	theta = 0;
	vertPos = 0;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < edgePoints; i++)
		{
			//for each vertex
			for (int v = 0; v < 4; v++)
			{
				if (v == 2)
				{
					theta += interval;
				}

				texCoords[vertPos++] = theta / (2 * PI); // texcoord u

				//set v based upon height
				if ((v == 1) || (v == 2))
				{
					texCoords[vertPos++] = 1 - (j / height); // texcoord v
				}
				else
				{
					texCoords[vertPos++] = 1 - ((j  + 1) / height); // texcoord v
				}
			}
		}
		theta = 0;
	}

	//generate a disc for the top or bottom
	if (hasBottom)
	{
		bottom = Disc(noEdgePoints);
	}
	if (hasTop)
	{
		glPushMatrix();
			glTranslatef(0, 1, 0);
			top = Disc(noEdgePoints);
		glPopMatrix();
	}
}

void Cylinder::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, verts);
	glNormalPointer(GL_FLOAT, 0, norms);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	//draw cylinder
	glDrawArrays(GL_QUADS, 0, vertices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//draw the discs if they are to be drawn
	if (hasBottom)
	{
		bottom.draw();
	}
	if (hasTop)
	{
		glPushMatrix();
		glTranslatef(0, 1, 0);
		top.draw();
		glPopMatrix();
	}
}