#include "Triangle.h"
#include "ShadersManager.h"
#include "Cam.h"

Triangle::Triangle(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3)
{
	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");

	_glBuffer = new GLBuffer(50, true, false, false);

	_glBuffer->glBegin(GL_TRIANGLES);
	_glBuffer->glColor3ub(255,0,0);
	_glBuffer->glVertex3f(v1.x, v1.y, v1.z);
	_glBuffer->glVertex3f(v2.x, v2.y, v2.z);
	_glBuffer->glVertex3f(v3.x, v3.y, v3.z);
	_glBuffer->glEnd();

	_vertexBufferID = _glBuffer->GetVertexBufferID();
	_colorBufferID = _glBuffer->GetColorBufferID();
	_vertexCount = _glBuffer->GetVertexCount();
}

void Triangle::Draw()
{
	_shaderProgram->Begin();

	GLint projMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "projMat");
	GLint viewMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "viewMat");
	GLint modelMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "modelMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, Cam::GetInstance()->projMat.m);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, Cam::GetInstance()->viewMat.m);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, _modelMat.m);

	GLuint colorLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
	glEnableVertexAttribArray(colorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexLoc);
	glDisableVertexAttribArray(colorLoc);

	_shaderProgram->End();
}

Triangle::~Triangle()
{
	if(_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}

	if (_glBuffer)
	{
		delete _glBuffer;
		_glBuffer = NULL;
	}
}