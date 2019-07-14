#include "GLTexture.h"
#include "GLUtil.h"
#include "TransformVertexBuf.h"
#include "ImageBuffer.h"
#include "Cam2D.h"
#include "ShadersManager.h"
#include "GLMemoryTrace.h"
#include "GLMemory.h"

GLTexture::GLTexture(int drawW, int drawH)
{
	_vertexBufferID = 0;
	_colorBufferID = 0;
	_vertexCount = 0;

	_drawW = drawW;
	_drawH = drawH;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/UVArray/UVArray.vs", "shaders/UVArray/UVArray.fs");
	GenerateBufferID();

	ImageBuffer* imgBuf = new ImageBuffer("data/Sample.png");
	_textureID = GLCreateTexture(imgBuf->GetWidth(), imgBuf->GetHeight(),
											imgBuf->GetBytesPerPixel(), imgBuf->GetBuffer());

	delete imgBuf;
}

void GLTexture::Draw()
{
	//glBindTexture(GL_TEXTURE_2D, _textureID);

	_shaderProgram->Begin();

	glm::mat4 mvp = Cam2D::GetInstance()->GetMVP(_modelMat.m);

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(mvp));

	GLuint uvLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "uv");
	glEnableVertexAttribArray(uvLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
	glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_TRUE, 0, (void*)0);

	GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);

	_shaderProgram->End();

	//glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::GenerateBufferID()
{
	_buffer = new GLBuffer(100, true, true, false);

	_buffer->glBegin(GL_TRIANGLE_STRIP);

	float w = _drawW;
	float h = _drawH;

	_buffer->glTexCoord2f(0, 1);
	_buffer->glVertex3f(0, 0, 0);

	_buffer->glTexCoord2f(1, 1);
	_buffer->glVertex3f(+w, 0, 0);

	_buffer->glTexCoord2f(0, 0);
	_buffer->glVertex3f(0, h, 0);

	_buffer->glTexCoord2f(1, 0);
	_buffer->glVertex3f(w, h, 0);

	_buffer->glEnd();

	_vertexBufferID = _buffer->GetVertexBufferID();
	_uvBufferID = _buffer->GetUVBufferID();
	_vertexCount = _buffer->GetVertexCount();
}

GLTexture::~GLTexture()
{
	if (_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}

	if (_textureID)
	{
		GLDeleteTexture(_textureID);
		_textureID = 0;
	}

	if (_buffer)
	{
		delete _buffer;
		_buffer = NULL;
	}
}