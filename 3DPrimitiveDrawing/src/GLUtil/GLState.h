#ifndef GLState_H
#define GLState_H

#include "GLInclude.h"

class GLState
{
private:

public:
	static void GLClearColor(float r, float g, float b, float a, GLfloat* prevColor);
	static GLboolean GLEnable(unsigned int state, GLboolean enable);
	static void GLEnable(unsigned int* arr, int size, bool enable);
	static GLfloat GLLineWidth(GLfloat width);
	static GLfloat GLPointSize(GLfloat val);
	static GLenum GLDepthFunc(GLenum val);
	static GLenum GLCullFace(GLenum val);
};

#endif
