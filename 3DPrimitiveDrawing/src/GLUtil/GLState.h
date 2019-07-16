#ifndef GLState_H
#define GLState_H

//TODO : Remove GLMat.h include and implemement GLInclude.h and include here
#include "GLMat.h"

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
};

#endif
