#include "Triangle2D.h"
#include "GLUtil/GLBuffer.h"

Triangle2D::Triangle2D(CVector3 v1, CVector3 v2, CVector3 v3)
{
	_shaderProgram = new ShaderProgram("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");

	GLBuffer* glBuffer = new GLBuffer(50, true, false, false);

	glBuffer->glBegin(GL_TRIANGLES);
	glBuffer->glColor4ub(255,0,0,255);
	glBuffer->glVertex3f(v1.x, v1.y, v1.z);
	glBuffer->glVertex3f(v2.x, v2.y, v2.z);
	glBuffer->glVertex3f(v3.x, v3.y, v3.z);

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

void Triangle2D::Draw()
{
	glPushMatrix();

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

Triangle2D::~Triangle2D()
{
	if(_shaderProgram)
	{
		delete _shaderProgram;
		_shaderProgram = NULL;
	}
}