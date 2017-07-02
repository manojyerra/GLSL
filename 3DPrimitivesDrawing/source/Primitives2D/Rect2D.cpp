#include "Rect2D.h"
#include "GLUtil/GLBatch.h"

Rect2D::Rect2D(float x, float y, float w, float h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	shaderProgram = new ShaderProgram("shaders/passThroughShader.vs", "shaders/passThroughShader.fs");

	GLBatch* batch = new GLBatch(50, false, false, false);

	batch->glBegin(GL_TRIANGLES);
	batch->glVertex3f(0.0f, 0.0f, 0.0f);
	batch->glVertex3f(1.0f, 0.0f, 0.0f);
	batch->glVertex3f(0.0f, 1.0f, 0.0f);
	batch->glVertex3f(1.0f, 0.0f, 0.0f);
	batch->glVertex3f(0.0f, 1.0f, 0.0f);
	batch->glVertex3f(1.0f, 1.0f, 0.0f);

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, batch->GetVertexCount() * 3 * 4, batch->GetVertexArr(), GL_STATIC_DRAW);

	if(batch)
	{
		delete batch;
		batch = NULL;
	}
}

void Rect2D::Draw()
{
	glPushMatrix();

	glTranslatef(_x, _y, 0);
	glScalef(_w, _h, 0);

	shaderProgram->Begin();

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	shaderProgram->End();

	glPopMatrix();
}

Rect2D::~Rect2D()
{
	if(shaderProgram)
	{
		delete shaderProgram;
		shaderProgram = NULL;
	}
}