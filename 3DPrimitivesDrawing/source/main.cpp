#include "DefinesAndIncludes.h"
#include <GL/freeglut.h>
#include "ShaderProgram.h"
#include "GLUtil/GLBatch.h"
#include "Primitives2D/Triangle2D.h"
#include "Primitives2D/Rect2D.h"
#include "Floor.h"
#include "Cam.h"

int CreateGlutWindow(char* title, int x, int y, int w, int h);
void Display();

Triangle2D* _triangle2D;
Rect2D* _rect2D;
Floor* _floor;
Cam* _cam;

float _sw = 500.0f;
float _sh = 500.0f;
float _zNear = 1.0f;
float _zFar = 10000.0f;
float _zNearPlaneW = 0.2f;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	CreateGlutWindow("First Window", 0,0,500,500);
	glewInit();

	//CVector3 v1(-1.0f, -1.0f, 0.0f);
	//CVector3 v2(1.0f, -1.0f, 0.0f);
	//CVector3 v3(0.0f,  1.0f, 0.0f);

	_cam = Cam::GetInstance();
	_cam->Init(500, 500, _zNear, _zFar, _zNearPlaneW);

	CVector3 v1(-1.0f, 1.0f, 0.0f);
	CVector3 v2(0.5f, 0.5f, 0.0f);
	CVector3 v3(0.0f, 0.5f, 0.0f);

	_triangle2D = new Triangle2D(v1, v2, v3);
	_rect2D = new Rect2D(-1, -1, 2, 1.98);

	_floor = new Floor();

	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}

int CreateGlutWindow(char* title, int x, int y, int w, int h)
{
	glutInitWindowPosition(x,y);
	glutInitWindowSize(w,h);
	glutCreateWindow(title);
	//glutReshapeWindow(w, h);
	//glutFullScreen();
	return 1;
}

void Display()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	_cam->SetProjection();
	_cam->SetModelViewMatrix();

	//_triangle2D->Draw();
	_rect2D->Draw();
	_floor->Draw();

	glutSwapBuffers();
}
