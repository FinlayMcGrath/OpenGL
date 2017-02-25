#include "Scene3D.h"

bool Scene3D::CreatePixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd = {0}; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Set the size of the structure
    pfd.nVersion = 1;							// Always set this to 1
	// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;			// standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
    pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
    pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
    pfd.cAccumBits = 0;							// nothing for accumulation
    pfd.cStencilBits = COLOUR_DEPTH;						// nothing for stencil

	//Gets a best match on the pixel format as passed in from device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == false ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
	//sets the pixel format if its ok. 
    if (SetPixelFormat(hdc, pixelformat, &pfd) == false) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
    return true;
}

void Scene3D::ResizeGLWindow(int width, int height)// Initialize The GL Window
{
	if (height == 0)// Prevent A Divide By Zero error
	{
		height = 1;// Make the Height Equal One
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate aspect ratio	
	gluPerspective(80.0f, (float)width / (float)height, 0.1, 500.0f);

	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();// Reset The Modelview Matrix
}

void Scene3D::InitializeOpenGL(int width, int height) 
{  
    hdc = GetDC(*hwnd);//  sets  global HDC

    if (!CreatePixelFormat(hdc))//  sets  pixel format
        PostQuitMessage (0);

    hrc = wglCreateContext(hdc);	//  creates  rendering context from  hdc
    wglMakeCurrent(hdc, hrc);		//	Use this HRC.

	ResizeGLWindow(width, height);	// Setup the Screen
}

void Scene3D::Resize()
{
	if(hwnd == NULL)
		return;

	GetClientRect(*hwnd, &screenRect);	
	ResizeGLWindow(screenRect.right, screenRect.bottom);	
}

void Scene3D::Init(HWND* wnd, Input* in)
{
	hwnd = wnd;
	input = in;

	GetClientRect(*hwnd, &screenRect);	//get rect into our handy global rect
	InitializeOpenGL(screenRect.right, screenRect.bottom); // initialise openGL

	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	//Also, do any other setting variables here for your app if you wish. 
	glEnable(GL_COLOR_MATERIAL);
	loadTextures();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_DEPTH_TEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	//texture filtering
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	
	room.init();
	room.setScale(250.0f, 250.0f, 250.0f);
	room.setPosition(0.0f, 0.0f, 0.0f);

	train.init();
	train.setPosition(125, 0.0f, 125);

	moveMode = freeMove;
	freeCamera.setPosition(125, 125, 125);
	portalRotVelocity = 36;

	http://www.turbosquid.com/FullPreview/Index.cfm/ID/662151
	fighter.Load("Models/fighter.obj", "textures/fighter.png");
	fighter.setPosition(100, 150, 100);
	mirror.setPosition(50, 50, 50);

	smallSphere.Load("Models/SmallSphere.obj", "Textures/Crate.png");
	teapot.Load("Models/Teapot.obj", "Textures/Crate.png");
}

void Scene3D::DrawScene(float dt) 
{
	HandleInput(dt);
	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

	glLoadIdentity();// load Identity Matrix

	// Where we are, What we look at, and which way is up
	gluLookAt(camera.getPosition().getX(), camera.getPosition().getY(), camera.getPosition().getZ(),
			camera.getLook().getX(), camera.getLook().getY(), camera.getLook().getZ(),
			camera.getUp().getX(), camera.getUp().getY(), camera.getUp().getZ());

	GLfloat mat_ambient[] = {0.7, 0.7, 0.7, 1.0};
	GLfloat mat_diffuse[] = {0.1, 0.5, 0.8, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat high_shininess[] = {100};
	GLfloat mat_emission[] = {0.2, 0.2, 0.2, 0.0};

	//set up material attributes
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	
	GLfloat Light_Ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat Light_Diffuse[] = {0.8, 0.8, 0.2, 1.0};
	GLfloat Light_Position[] = {125, 200, 125, 1.0};
	GLfloat Light_Specular[] = {1.0, 1.0, 1.0, 1.0};
	light.init(GL_LIGHT3, Light_Ambient, Light_Diffuse, Light_Position, Light_Specular, 1.0f, -0.125f, -1.0f);
	glEnable(GL_LIGHT3);
	
	GLfloat shininess[] = {100.0};
	GLfloat highSpec[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, highSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);	

	glPushMatrix();	
		//draw skybox
		glPushMatrix();		
			glTranslatef(camera.getPosition().getX(), camera.getPosition().getY(), camera.getPosition().getZ());
			glScalef(5, 5, 5);
			glDisable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D, skybox);	//tells opengl which texture to use
			drawSkybox();
			glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glDisable(GL_TEXTURE_2D);
		//draw any bullets fired by the fighter
		for (deque<Bullet>::iterator it = lasers.begin(); it != lasers.end(); it++)
		{
			it->update(dt);
			glPushMatrix();
				glTranslatef(it->getPosition().getX(), it->getPosition().getY(), it->getPosition().getZ());
				glRotatef(it->getRotation().getY(), 0, 1, 0);
				glRotatef(it->getRotation().getX(), 1, 0, 0);
				glRotatef(it->getRotation().getZ(), 0, 0, 1);
				glScalef(it->getScale().getX(), it->getScale().getY(), it->getScale().getZ());
				it->draw();
			glPopMatrix();
		}		
		glEnable(GL_TEXTURE_2D);

		//REFLECTION BEGIN
		glPushMatrix();		
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glDisable(GL_DEPTH_TEST);
			glPushMatrix();	
				glTranslatef(50, 50, 50);
				glScalef(10.0f, 10.0f, 10.0f);
				glColor3f(0.5f, 0.5f, 0.5f);
				renderQuad();
			glPopMatrix();

			glEnable(GL_DEPTH_TEST);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glPushMatrix();
				glScalef(1.0, -1.0, 1.0);
				glTranslatef(0, 20, 0);
				glRotatef(35, 0, 1, 0);
				glBindTexture(GL_TEXTURE_2D, crate);
				mirror.draw();
			glPopMatrix();
			glDisable(GL_STENCIL_TEST);
		
			glPushMatrix();
				glTranslatef(0, 20, 0);
				glRotatef(35, 0, 1, 0);
				mirror.draw();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	// reset colour
	glColor3f(1.0f, 1.0f, 1.0f);
	drawShadowMatrixScene();

	//creates a rotating portal texture
	glPushMatrix();
		glEnable(GL_BLEND);
		glTranslatef(50.0f, 50.0f, 50.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(50.0f, 50.0f, 50.0f);

		glMatrixMode(GL_TEXTURE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, portalTex);
			portalRot += portalRotVelocity * dt;
		
			glPushMatrix();
				//translate to object's local origin, rotate around that origin then translate back to original position
				glTranslatef(0.5f, 0.5f, 0.0f);
				glRotatef(portalRot, 0.0f, 0.0f, 1.0f);	
				glTranslatef(-0.5f, -0.5f, 0.0f);
				renderQuad();
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		glDisable(GL_BLEND);
	glPopMatrix();
	
	// reset colour
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, fighterTex);
	fighter.draw();	

	// reset colour
	glColor3f(1.0f, 1.0f, 1.0f);

	SwapBuffers(hdc);// Swap the frame buffers.
}	

//draws a square
void Scene3D::renderQuad()
{	
	glBegin (GL_TRIANGLES);
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1, 0, -1);
                        
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1, 0, -1);

		glNormal3f( 0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 0, 1);

		glNormal3f( 0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1, 0, -1);
                        
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1, 0, 1);
                        
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 0, 1);
	glEnd();
}

void Scene3D::HandleInput(float dt)
{		
	//swaps what camera/object is being controlled, train, camera or fighter
	if (input->isKeyDown('1'))
	{
		moveMode = trainMove;
	}

	if (input->isKeyDown('2'))
	{
		moveMode = freeMove;
	}

	if (input->isKeyDown('3'))
	{
		moveMode = fighterMove;
	}

	switch(moveMode)
	{
		//if the camera can move anywhere
	case freeMove:	
		//turn camera left or right with mouse
		if ((input->getMouseX() < (SCREEN_WIDTH * 0.25)) || (input->getMouseX() > (SCREEN_WIDTH * 0.75)))
		{
			freeCamera.setRotation(freeCamera.getRotation().getX(), 
								freeCamera.getRotation().getY() + 
								(dt * freeCamera.getRotationalVelocity() * ((input->getMouseX() - (SCREEN_WIDTH / 2.0)) / (SCREEN_WIDTH / 2.0))), 
								freeCamera.getRotation().getZ());
		}

		//turn camera up or down with mouse
		if ((input->getMouseY() < (SCREEN_HEIGHT * 0.25)) || (input->getMouseY() > (SCREEN_HEIGHT * 0.75)))
		{
			freeCamera.setRotation(freeCamera.getRotation().getX() - 
								(dt * freeCamera.getRotationalVelocity() * ((input->getMouseY() - (SCREEN_HEIGHT / 2.0)) / (SCREEN_HEIGHT / 2.0))), 
								freeCamera.getRotation().getY(), 
								freeCamera.getRotation().getZ());
		}

		//move camera forward
		if (input->isKeyDown('W'))
		{
			freeCamera.setPosition(freeCamera.getPosition().getX() + (freeCamera.getForward().getX() * dt * freeCamera.getVelocity()), 
							   freeCamera.getPosition().getY() + (freeCamera.getForward().getY() * dt * freeCamera.getVelocity()), 
							   freeCamera.getPosition().getZ() + (freeCamera.getForward().getZ() * dt * freeCamera.getVelocity()));
		}

		//move camera back
		if (input->isKeyDown('S'))
		{
			freeCamera.setPosition(freeCamera.getPosition().getX() + (-freeCamera.getForward().getX() * dt * freeCamera.getVelocity()), 
								freeCamera.getPosition().getY() + (-freeCamera.getForward().getY() * dt * freeCamera.getVelocity()), 
								freeCamera.getPosition().getZ() + (-freeCamera.getForward().getZ() * dt * freeCamera.getVelocity()));
		}

		//move camera left
		if (input->isKeyDown('A'))
		{
			freeCamera.setPosition(freeCamera.getPosition().getX() + (-freeCamera.getRight().getX() * dt * freeCamera.getVelocity()), 
								freeCamera.getPosition().getY() + (-freeCamera.getRight().getY() * dt * freeCamera.getVelocity()), 
								freeCamera.getPosition().getZ() + (-freeCamera.getRight().getZ() * dt * freeCamera.getVelocity()));
		}

		//move camera right
		if (input->isKeyDown('D'))
		{
			freeCamera.setPosition(freeCamera.getPosition().getX() + (freeCamera.getRight().getX() * dt * freeCamera.getVelocity()), 
								freeCamera.getPosition().getY() + (freeCamera.getRight().getY() * dt * freeCamera.getVelocity()), 
								freeCamera.getPosition().getZ() + (freeCamera.getRight().getZ() * dt * freeCamera.getVelocity()));
		}

		//move camera up
		if (input->isKeyDown(VK_SPACE))
		{
			freeCamera.setPosition(freeCamera.getPosition().getX(), 
								freeCamera.getPosition().getY() + (-freeCamera.getForward().getY() * dt * freeCamera.getVelocity()), 
								freeCamera.getPosition().getZ());
		}
		//move camera down if space and control is pressed
		else if (input->isKeyDown(VK_SPACE) && input->isKeyDown(VK_CONTROL))
		{
			freeCamera.setPosition(freeCamera.getPosition().getX(), 
								freeCamera.getPosition().getY() + (freeCamera.getForward().getY() * dt * freeCamera.getVelocity()), 
								freeCamera.getPosition().getZ());
		}

		//shine a light pointing towards what the camera is looking at
		if (input->getLeftClicked())
		{		
			GLfloat Light_Ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
			GLfloat Light_Diffuse[] = {0.8, 0.8, 0.2, 1.0};
			GLfloat Light_Position[] = {freeCamera.getPosition().getX() + freeCamera.getForward().getX(),
										freeCamera.getLook().getY() + freeCamera.getForward().getY(), 
										freeCamera.getLook().getZ() + freeCamera.getForward().getZ(), 0.0};
			GLfloat Light_Specular[] = {1.0, 1.0, 1.0, 1.0};
			light.init(GL_LIGHT4, Light_Ambient, Light_Diffuse, Light_Position, Light_Specular, 1.0f, 0.125f, 0.0f);
			glEnable(GL_LIGHT4);
		}
		else
		{
			//disable that light
			glDisable(GL_LIGHT4);
		}
		
		freeCamera.update();
		camera = freeCamera;
		break;

	case trainMove:	
		//move train forward
		if (input->isKeyDown('W'))
		{
			train.setPosition(train.getPosition().getX() + (dt * train.getVelocity() * train.getForward().getX()), 
							  train.getPosition().getY(),
							  train.getPosition().getZ() + (dt * train.getVelocity() * train.getForward().getZ()));
		}

		//move train back
		if (input->isKeyDown('S'))
		{
			train.setPosition(train.getPosition().getX() - (dt * train.getVelocity() * train.getForward().getX()), 
							  train.getPosition().getY(),
							  train.getPosition().getZ() - (dt * train.getVelocity() * train.getForward().getZ()));
		}

		//turn train anticlockwise on x-z plane
		if (input->isKeyDown('A'))
		{
			train.setRotation(0.0f, train.getRotation().getY() - (train.getRotationalVelocity() * dt), 0.0f);
		}
		
		//turn train clockwise on x-z plane
		if (input->isKeyDown('D'))
		{
			train.setRotation(0.0f, train.getRotation().getY() + (train.getRotationalVelocity() * dt), 0.0f);
		}

		
		train.update();
		camera = *train.getCamera();

		break;
	case fighterMove:
		//turn fighter to face left or right
		if ((input->getMouseX() < (SCREEN_WIDTH * 0.25)) || (input->getMouseX() > (SCREEN_WIDTH * 0.75)))
		{
			fighter.setRotation(fighter.getRotation().getX(),
								fighter.getRotation().getY() -
								(dt * fighter.getRotationalVelocity() * (fighter.getUp().getY() - fighter.getForward().getY()) * ((input->getMouseX() - (SCREEN_WIDTH / 2.0)) / (SCREEN_WIDTH / 2.0))),
								fighter.getRotation().getZ());
		}

		//turn fighter to face up or down
		if ((input->getMouseY() < (SCREEN_HEIGHT * 0.25)) || (input->getMouseY() > (SCREEN_HEIGHT * 0.75)))
		{
			fighter.setRotation(fighter.getRotation().getX() + 
								(dt * fighter.getRotationalVelocity() * (fighter.getUp().getX() - fighter.getForward().getX()) * ((input->getMouseY() - (SCREEN_HEIGHT / 2.0)) / (SCREEN_HEIGHT / 2.0))),
								fighter.getRotation().getY(),
								fighter.getRotation().getZ());
		}

		//fire a laser on left click
		if (input->getLeftClick())
		{
			input->setLeftClick(false);
			lasers.push_back(fighter.fire());
			if (lasers.size() > 10)
			{
				lasers.pop_front();
			}
		}

		//move fighter forward
		if (input->isKeyDown('W'))
		{
			fighter.setPosition(fighter.getPosition().getX() + (fighter.getForward().getX() * dt * fighter.getVelocity()), 
							   fighter.getPosition().getY() + (fighter.getForward().getY() * dt * fighter.getVelocity()), 
							   fighter.getPosition().getZ() + (fighter.getForward().getZ() * dt * fighter.getVelocity()));
		}

		//move fighter back
		if (input->isKeyDown('S'))
		{
			fighter.setPosition(fighter.getPosition().getX() - (fighter.getForward().getX() * dt * fighter.getVelocity()), 
								fighter.getPosition().getY() - (fighter.getForward().getY() * dt * fighter.getVelocity()), 
								fighter.getPosition().getZ() - (fighter.getForward().getZ() * dt * fighter.getVelocity()));
		}

		//move fighter left
		if (input->isKeyDown('A'))
		{
			fighter.setPosition(fighter.getPosition().getX() - (fighter.getRight().getX() * dt * fighter.getVelocity()), 
								fighter.getPosition().getY() - (fighter.getRight().getY() * dt * fighter.getVelocity()), 
								fighter.getPosition().getZ() - (fighter.getRight().getZ() * dt * fighter.getVelocity()));
		}

		//move fighter right
		if (input->isKeyDown('D'))
		{
			fighter.setPosition(fighter.getPosition().getX() + (fighter.getRight().getX() * dt * fighter.getVelocity()), 
								fighter.getPosition().getY() + (fighter.getRight().getY() * dt * fighter.getVelocity()), 
								fighter.getPosition().getZ() + (fighter.getRight().getZ() * dt * fighter.getVelocity()));
		}
		fighter.update();
		camera = *fighter.getCamera();
		break;
	}

	//turn wireframe mode on or off
	static bool enterPressed = false;
	static bool drawMode = true;
	if (input->isKeyDown(VK_RETURN) && !enterPressed)
	{
		drawMode = !drawMode;
	}

	if (drawMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	enterPressed = input->isKeyDown(VK_RETURN);
}

//this function draws a cube with specific texture coordinates that make up a skybox
void Scene3D::drawSkybox()
{
	//CUBE
	glBegin (GL_TRIANGLES);//Begin drawing state
		
		//front
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.25f);
		glVertex3f(	-0.5, 0.5, 0.5);

		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f( 0.5, -0.5, 0.5);
		
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.25f, 0.25f);
		glVertex3f( 0.5, 0.5, 0.5);			
		
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.25f);
		glVertex3f(	-0.5, 0.5, 0.5);	
		
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.5f);
		glVertex3f( -0.5, -0.5, 0.5);		
		
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f( 0.5, -0.5, 0.5);
				
		//left	
		
		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.25f);
		glVertex3f(	-0.5, 0.5, 0.5);	

		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.75f, 0.25f);	
		glVertex3f(	-0.5, 0.5, -0.5);	

		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.75f, 0.5f);	
		glVertex3f(	-0.5, -0.5, -0.5);
		
		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.25f);
		glVertex3f(	-0.5, 0.5, 0.5);	

		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.75f, 0.5f);
		glVertex3f(	-0.5, -0.5, -0.5);	

		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.5f);
		glVertex3f( -0.5, -0.5, 0.5);		
		
		//top

		glNormal3f( 0.0f, 1.0f, 0.0f);	
		glTexCoord2f(0.25f, 0.0f);
		glVertex3f(	-0.5, 0.5, 0.5);

		glNormal3f( 0.0f, 1.0f, 0.0f);	
		glTexCoord2f(0.5f, 0.0f);
		glVertex3f(	-0.5, 0.5, -0.5);	

		glNormal3f( 0.0f, 1.0f, 0.0f);	
		glTexCoord2f(0.5f, 0.25f);	
		glVertex3f(	0.5, 0.5, -0.5);		

		glNormal3f( 0.0f, 1.0f, 0.0f);	
		glTexCoord2f(0.25f, 0.0f);
		glVertex3f(	-0.5, 0.5, 0.5);	

		glNormal3f( 0.0f, 1.0f, 0.0f);	
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(	0.5, 0.5, -0.5);

		glNormal3f( 0.0f, 1.0f, 0.0f);	
		glTexCoord2f(0.25f, 0.25f);		
		glVertex3f(	0.5, 0.5, 0.5);

		//right	

		glNormal3f( 1.0f, 0.0f, 0.0f);		
		glTexCoord2f(0.5f, 0.5f);		
		glVertex3f(	0.5, -0.5, -0.5);

		glNormal3f( 1.0f, 0.0f, 0.0f);	
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(	0.5, 0.5, -0.5);

		glNormal3f( 1.0f, 0.0f, 0.0f);	
		glTexCoord2f(0.25f, 0.25f);
		glVertex3f( 0.5, 0.5, 0.5);		

		glNormal3f( 1.0f, 0.0f, 0.0f);	
		glTexCoord2f(0.5f, 0.5f);	
		glVertex3f(	0.5, -0.5, -0.5);

		glNormal3f( 1.0f, 0.0f, 0.0f);	
		glTexCoord2f(0.25f, 0.25f);	
		glVertex3f( 0.5, 0.5, 0.5);

		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.25f, 0.5f);	
		glVertex3f( 0.5, -0.5, 0.5);

		//bottom		

		glNormal3f( 0.0f, -1.0f, 0.0f);		
		glTexCoord2f(0.5f, 0.75f);			
		glVertex3f(	0.5, -0.5, -0.5);

		glNormal3f( 0.0f, -1.0f, 0.0f);	
		glTexCoord2f(0.25f, 0.75f);	
		glVertex3f(	-0.5, -0.5, -0.5);	

		glNormal3f( 0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.25f, 0.5f);		
		glVertex3f(	-0.5, -0.5, 0.5);
		
		glNormal3f( 0.0f, -1.0f, 0.0f);		
		glTexCoord2f(0.5f, 0.75f);
		glVertex3f(	0.5, -0.5, -0.5);

		glNormal3f( 0.0f, -1.0f, 0.0f);		
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f(	-0.5, -0.5, 0.5);

		glNormal3f( 0.0f, -1.0f, 0.0f);	
		glTexCoord2f(0.5f, 0.5f);	
		glVertex3f(	0.5, -0.5, 0.5);	

		//back

		glNormal3f( 0.0f, 0.0f, -1.0f);		
		glTexCoord2f(0.5f, 0.5f);				
		glVertex3f(	0.5, -0.5, -0.5);	

		glNormal3f( 0.0f, 0.0f, -1.0f);	
		glTexCoord2f(0.75f, 0.5f);
		glVertex3f(	-0.5, -0.5, -0.5);	

		glNormal3f( 0.0f, 0.0f, -1.0f);	
		glTexCoord2f(0.75f, 0.25f);
		glVertex3f(	-0.5, 0.5, -0.5);		

		glNormal3f( 0.0f, 0.0f, -1.0f);		
		glTexCoord2f(0.5f, 0.5f);
		glVertex3f(	0.5, -0.5, -0.5);

		glNormal3f( 0.0f, 0.0f, -1.0f);		
		glTexCoord2f(0.75f, 0.25f);
		glVertex3f(	-0.5, 0.5, -0.5);	

		glNormal3f( 0.0f, 0.0f, -1.0f);	
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(	0.5, 0.5, -0.5);	

	glEnd();//end drawing
}

//loads all textures
void Scene3D::loadTextures()
{
	avengers = SOIL_load_OGL_texture
	(
		"textures/aTrans.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	triangle = SOIL_load_OGL_texture
	(
		"textures/triangle.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	crate = SOIL_load_OGL_texture
	(
		"textures/crate.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	skybox = SOIL_load_OGL_texture
	(
		"textures/Lab 17 Skybox Image.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	
	grass = SOIL_load_OGL_texture
	(
		"textures/grass1.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	chess = SOIL_load_OGL_texture
	(
		"textures/checked.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	
	imposter = SOIL_load_OGL_texture
	(
		"textures/imposter.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	
	fighterTex = SOIL_load_OGL_texture
	(
		"textures/fighter.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	http://s158.photobucket.com/user/Psychomaxx/media/metroid/dark_portal.png.html
	portalTex = SOIL_load_OGL_texture
	(
		"textures/portal.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

void Scene3D::drawShadowMatrixScene()
{
	// Activate light
	glLightfv(train.getLight().getLightID(), GL_AMBIENT,  train.getLight().getAmbient());
	glLightfv(train.getLight().getLightID(), GL_DIFFUSE,  train.getLight().getDiffuse());
	glLightfv(train.getLight().getLightID(), GL_POSITION, train.getLight().getPosition());
	glEnable(train.getLight().getLightID());

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
	generateShadowMatrix(shadowMatrix, train.getLight().getPosition(), v0, v1, v2);
	
	glPushMatrix();
		glScalef(250, 250, 250);
		glBindTexture(GL_TEXTURE_2D, crate);
		room.draw();
	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.1f, 0.1f, 0.1f); // Shadow's color
    glPushMatrix();
		glMultMatrixf((GLfloat *) shadowMatrix);
		train.draw();
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	//render object
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, crate);
		train.draw();
	glPopMatrix();
}

void Scene3D::generateShadowMatrix(float matrix[16], float light_pos[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3] ) 
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
	