void Scene3D::drawShadowMatrixScene()
{
	GLfloat Light_Ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat Light_Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat Light_Position[] = {4.0, 10.0, 0.0, 1.0};

	// Activate light
	glLightfv(GL_LIGHT0, GL_AMBIENT,  Light_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
	glEnable(GL_LIGHT0);

    GLfloat v0[3], v1[3], v2[3];

    // To define a plane that matches the floor, we need to 3 vertices from it
    v0[0] = -1.0;
    v0[1] = -1.0;
    v0[2] = -1.0;

    v1[0] = -1.0;
    v1[1] = -1.0;
    v1[2] = 1.0;

    v2[0] = 1.0;
    v2[1] = -1.0;
    v2[2] = -1.0;

     
    // Build a shadow matrix using the light's current position and the plane
	generateShadowMatrix(mshadowMatrix, Light_Position, v0, v1, v2);

	//draw floor
	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-5.0, -1.0, -5.0);
		glVertex3f(-5.0, -1.0, 5.0);
		glVertex3f(5.0, -1.0, 5.0);
		glVertex3f(5.0, -1.0, -5.0);
	glEnd();

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.1f, 0.1f, 0.1f); // Shadow's color
    glPushMatrix();
		glMultMatrixf((GLfloat *) mshadowMatrix);
		//translate to floor and draw shadow, remember to match any transforms on the object
		glTranslatef(0, 1, 0);
		glRotatef(rotation, 0, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		model.Render();
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	//render object
	glPushMatrix();
		glTranslatef(0, 1, 0);
		glRotatef(rotation, 0, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		model.Render();
	glPopMatrix();
}

void Scene3D::generateShadowMatrix(float matrix[16], float light_pos[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]) 
{

//Defining vertices of plane are PQR with edges PQ and PR
Vec3 P(v0[0], v0[1], v0[2]);
Vec3 Q(v1[0], v1[1], v1[2]);
Vec3 R(v2[0], v2[1], v2[2]);

Vec3 PQ = (Q - P).normalize();
Vec3 PR = (R - P).normalize();
Vec3 normal = PR.cross(PQ);

//Equation of plane is ax + by + cz = d
//a, b and c are the coefficients of the normal to the plane (i.e. normal = ai + bj + ck)
//If (x0, y0, z0) is any point on the plane, d = a*x0 + b*y0 + c*z0
//i.e. d is the dot product of any point on the plane (using P here) and the normal to the plane
float a, b, c, d;
a = normal.getX();
b = normal.getY();
c = normal.getZ();
d = normal.dot(P);

//Origin of projection is at x, y, z. Projection here originating from the light source's position
float x, y, z;

x = light_pos[0];
y = light_pos[1];
z = light_pos[2];

//This is the general perspective transformation matrix from a point (x, y, z) onto the plane ax + by + cz = d
matrix[0] = d - (b * y + c * z);
matrix[1] = a * y;
matrix[2] = a * z;
matrix[3] = a;

matrix[4] = b * x;
matrix[5] = d - (a * x + c * z);
matrix[6] = b * z;
matrix[7] = b;

matrix[8] = c * x;
matrix[9] = c * y;
matrix[10] = d - (a * x + b * y);
matrix[11] = c;

matrix[12] = -d * x;
matrix[13] = -d * y;
matrix[14] = -d * z;
matrix[15] = -(a * x + b * y + c * z);
}