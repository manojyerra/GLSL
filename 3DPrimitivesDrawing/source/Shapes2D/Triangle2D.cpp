#include "Triangle2D.h"
#include "GLUtil/GLBatch.h"

Triangle2D::Triangle2D(CVector3 v1, CVector3 v2, CVector3 v3)
{
	shaderProgram = new ShaderProgram("shaders/passThroughShader.vs", "shaders/passThroughShader.fs");

	GLBatch* batch = new GLBatch(50, false, false, false);

	batch->glBegin(GL_TRIANGLES);
	batch->glVertex3f(v1.x, v1.y, v1.z);
	batch->glVertex3f(v2.x, v2.y, v2.z);
	batch->glVertex3f(v3.x, v3.y, v3.z);

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, batch->GetVertexCount() * 3 * 4, batch->GetVertexArr(), GL_STATIC_DRAW);

	if(batch)
	{
		delete batch;
		batch = NULL;
	}
}

void Triangle2D::Draw()
{
	shaderProgram->Begin();

	//shaderProgram->SetUniform4fv("uColor", 0.0f, 1.0f, 0.0f, 1.0f);

	glUniform4f( glGetUniformLocation(shaderProgram->ProgramID(), "uColor"), 0.0f, 1.0f, 0.0f, 1.0f );

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	shaderProgram->End();
}

Triangle2D::~Triangle2D()
{
	if(shaderProgram)
	{
		delete shaderProgram;
		shaderProgram = NULL;
	}
}