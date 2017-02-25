#ifndef SCENE3D_H
#define SCENE3D_H

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <deque>
#include "Input.h"
#include "SOIL.h"
#include "Camera.h"
#include "Cube.h"
#include "Sphere.h"
#include "Model.h"
#include "Train.h"
#include "Bullet.h"
#include "Fighter.h"

#define COLOUR_DEPTH 16	//Colour depth
#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 1440
#define PI 3.14159

struct ShadowCaster
{
	std::vector<Vec3> verts; // shadow casting object verts
	Vec3 normal;	// normal
	GLuint shadowVolume; //display list for shadow volume
};

enum movementMode {freeMove, trainMove, fighterMove};

class Scene3D
{
public:
	Scene3D(){};
	~Scene3D(){};
	void Init(HWND*, Input*);	//initialse function
	void DrawScene(float);	// render scene
	void Resize();

protected:
	bool CreatePixelFormat(HDC);
	void ResizeGLWindow(int width, int height);	
	void InitializeOpenGL(int width, int height);
	void HandleInput(float dt);
	void drawSkybox();
	void loadTextures();
	void renderQuad();

	//shadows functions
	void generateShadowMatrix(float matrix[16], float light_pos[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
	void drawShadowMatrixScene();

	float shadowMatrix[16];

	//vars
	HWND* hwnd;
	Input* input;
	RECT screenRect;
	HDC	hdc;
	HGLRC hrc;			//hardware RENDERING CONTEXT	

	Camera camera, freeCamera;

	//textures
	GLuint avengers, triangle, crate, skybox, grass, chess, imposter, fighterTex, portalTex;

	GLuint Dlist;

	//procedural shapes
	Train train;
	Cube room;

	deque<Bullet> lasers;

	//models
	Model smallSphere, teapot;

	Fighter fighter, mirror;

	//misc variables
	ShadowCaster shadowCaster;

	movementMode moveMode;

	Light light;
	float portalRotVelocity, portalRot;
};

#endif