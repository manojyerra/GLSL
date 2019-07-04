#include "Triangle.h"
#include "GLBuffer.h"
#include "ShadersManager.h"
#include "Cam.h"

Triangle::Triangle(CVector3& v1, CVector3& v2, CVector3& v3)
{
	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");

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

void Triangle::Draw()
{
	_shaderProgram->Begin();

	GLint projMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "projMat");
	GLint modelMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "modelMat");
	GLint oriMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "oriMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, Cam::GetInstance()->projMat.m);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, Cam::GetInstance()->modelMat.m);
	glUniformMatrix4fv(oriMatLoc, 1, GL_FALSE, _oriMat.m);

	GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
	glEnableVertexAttribArray(colorID);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribPointer(colorID, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);

	GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexID);
	glDisableVertexAttribArray(colorID);

	_shaderProgram->End();
}

Triangle::~Triangle()
{
	if(_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}