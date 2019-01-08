#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ShaderProgram.h"
#include "ImageBuffer.h"
#include "Input.h"
#include "GLMat.h"
#include "Floor.h"
#include "Box.h"
#include "Cam.h"
#include "Sphere.h"

int CreateGlutWindow(char* title, int x, int y, int w, int h);
void Display();
void MouseInput(int button, int updown, int x, int y);

GLuint vertexbuffer;
GLuint uvbuffer;

int frameW;
int frameH;
ImageBuffer* _img;
GLuint _textureID;

Box* _box; 
Sphere* _sphere;
Floor* _floor;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	CreateGlutWindow("First Window", 0,0,1000,700);
	glewInit();

	float zNear = 1.0f;
	float zFar = 10000.0f;
	float zNearPlaneW = 1.0f;
	float zNearPlaneHalfW = zNearPlaneW / 2.0f;

	Cam::GetInstance()->Init(frameW, frameH, zNear, zFar, zNearPlaneHalfW);
	
	_floor = new Floor();
	_box = new Box(0,0,0, 4,4,4);
	_sphere = new Sphere(0, 0, 0, 5);

	glutDisplayFunc(Display);
	glutMouseFunc(MouseInput);
	glutMainLoop();

	return 0;
}

int CreateGlutWindow(char* title, int x, int y, int w, int h)
{
	glutInitWindowPosition(x,y);
	glutInitWindowSize(w,h);

	frameW = w;
	frameH = h;

	return glutCreateWindow(title);
}

void MouseInput(int button, int updown, int x, int y)
{
	//printf("\nButton = %d, updown = %d, x = %d, y = %d", button, updown, x, y);

	if (button == 0)
	{
		Input::LEFT_BUTTON_DOWN = (updown == 0);
	}
	else if (button == 1)
	{
		Input::MIDDLE_BUTTON_DOWN = (updown == 0);
	}
	else if (button == 2)
	{
		Input::RIGHT_BUTTON_DOWN = (updown == 0);
	}
}


void Display()
{
	Input::Update(1.0f/30.0f);

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam::GetInstance()->UpdateCamera();
	Cam::GetInstance()->SetModelViewMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SHADE_MODEL);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	//_floor->Draw();
	//_box->Draw();
	_sphere->Draw();

	glutSwapBuffers();
	glutPostRedisplay();
}

