#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "glsl/glsl.h"

int CreateGlutWindow(char* title, int x, int y, int w, int h);
void Display();

glShaderManager shaderMgr;
glShader *shader;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int windowID = CreateGlutWindow("First Window", 0,0,500,500);

	shader = shaderMgr.loadfromFile("shaders/vertexshader.txt","shaders/fragmentshader.txt"); // load (and compile, link) from file

	if(shader==0) 
		std::cout << "Error Loading, compiling or linking shader\n";

	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}

int CreateGlutWindow(char* title, int x, int y, int w, int h)
{
	glutInitWindowPosition(x,y);
	glutInitWindowSize(w,h);
	return glutCreateWindow(title);
}

void Display()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(shader) 
		shader->begin();

    glutSolidSphere(1.0,32,32);
    
	if(shader)
		shader->end();

	glutSwapBuffers();
}
