#include "GLState.h"

void GLState::GLClearColor(float r, float g, float b, float a, GLfloat* prevColor)
{
	glGetFloatv(GL_COLOR_CLEAR_VALUE, prevColor);
	glClearColor(r, g, b, a);
}

GLboolean GLState::GLEnable(unsigned int state, GLboolean enable)
{
	GLboolean isEnabled = glIsEnabled(state);

	if (enable)
		glEnable(state);
	else
		glDisable(state);

	return isEnabled;
}

void GLState::GLEnable(unsigned int* statesAarr, int size, bool enable)
{
	if (enable)
	{
		for (int i = 0; i < size; i++)
			glEnable(statesAarr[i]);
	}
	else
	{
		for (int i = 0; i < size; i++)
			glDisable(statesAarr[i]);
	}
}

GLfloat GLState::GLLineWidth(GLfloat width)
{
	GLfloat lineWidth = 1;
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);
	glLineWidth(width);
	return lineWidth;
}

GLfloat GLState::GLPointSize(GLfloat val)
{
	GLfloat pointSize = 1.0f;
	glGetFloatv(GL_POINT_SIZE, &pointSize);
	glPointSize(val);
	return pointSize;
}

GLenum GLState::GLDepthFunc(GLenum val)
{
	int returnVal = 0;
	glGetIntegerv(GL_DEPTH_FUNC, &returnVal);
	glDepthFunc(val);
	return returnVal;
}