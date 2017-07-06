#include "Rect2D.h"
#include "GLUtil/GLBuffer.h"

Rect2D::Rect2D(float x, float y, float w, float h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_shaderProgram = new ShaderProgram("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");

	GLBuffer* glBuffer = new GLBuffer(50, true, false, false);

	glBuffer->glBegin(GL_TRIANGLES);

	glBuffer->glColor4ub(255,0,0,255);

	glBuffer->glVertex3f(0.0f, 0.0f, 0.0f);
	glBuffer->glVertex3f(1.0f, 0.0f, 0.0f);
	glBuffer->glVertex3f(0.0f, 1.0f, 0.0f);

	glBuffer->glVertex3f(1.0f, 0.0f, 0.0f);
	glBuffer->glVertex3f(0.0f, 1.0f, 0.0f);
	glBuffer->glVertex3f(1.0f, 1.0f, 0.0f);

	glBuffer->glEnd();

	_vertexBufferID = glBuffer->GetVertexBufferID();
	_colorBufferID = glBuffer->GetColorBufferID();
	_vertexCount = glBuffer->GetVertexCount();

	if(glBuffer)
	{
		delete glBuffer;
		glBuffer = NULL;
	}
}

void Rect2D::Draw()
{
	glPushMatrix();

	glTranslatef(_x, _y, 0);
	glScalef(_w, _h, 0);

	_shaderProgram->Begin();

	GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
	glEnableVertexAttribArray(colorID);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribPointer(colorID, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);

	GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	_shaderProgram->End();

	glPopMatrix();
}

Rect2D::~Rect2D()
{
	if(_shaderProgram)
	{
		delete _shaderProgram;
		_shaderProgram = NULL;
	}
}