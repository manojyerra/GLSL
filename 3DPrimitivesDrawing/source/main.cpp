#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ShaderProgram.h"

int CreateGlutWindow(char* title, int x, int y, int w, int h);
void Display();

GLuint vertexbuffer;

static const GLfloat g_vertex_buffer_data[] =
{
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

ShaderProgram* shaderProgram;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	CreateGlutWindow("First Window", 0,0,500,500);
	glewInit();

	shaderProgram = new ShaderProgram("shaders/passThroughShader.vs", "shaders/passThroughShader.fs");

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

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
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.1, 0, 0);

	shaderProgram->Begin();

	shaderProgram->SetUniform4fv("uColor", 0.0f, 1.0f, 0.0f, 1.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	shaderProgram->End();

	glutSwapBuffers();
}
