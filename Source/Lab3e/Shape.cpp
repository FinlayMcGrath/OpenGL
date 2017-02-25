#include "shape.h"

void Shape::init(Vec3 position_, Vec3 rotation_, Vec3 scale_)
{
	position = position_;
	rotation = rotation_;
	scale = scale_;
	init();
}

void Shape::render(int vertices)
{
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer (3, GL_FLOAT, 0, verts);
	glNormalPointer (GL_FLOAT, 0, norms);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	
	float interval = (2 * PI) / vertices;
	float theta = 0;
	for (int i = 0; i < vertices; i++)
	{
		float x = cos(theta);
		float y = sin(theta);

		theta += interval; 
	}

	// add code to render the cube (above) using method 2
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}