#include "Texture2D.h"
#include "GLUtil/GLBuffer.h"
#include "ImageBuffer.h"

Texture2D::Texture2D(string texturePath, float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	_shaderProgram = new ShaderProgram("shaders/ImageFilters/ImageFilters.vs", "shaders/ImageFilters/ImageFilters.fs");

	_textureWidth = 0;
	_textureHeight = 0;
	_textureID = LoadFile(texturePath);

	GLBuffer* glBuffer = new GLBuffer(50, false, true, false);

	glBuffer->glBegin(GL_TRIANGLES);

	glBuffer->glTexCoord2f(0.0f, 0.0f);
	glBuffer->glVertex3f(0.0f, 0.0f, 0.0f);

	glBuffer->glTexCoord2f(1.0f, 0.0f);
	glBuffer->glVertex3f(1.0f, 0.0f, 0.0f);

	glBuffer->glTexCoord2f(0.0f, 1.0f);
	glBuffer->glVertex3f(0.0f, 1.0f, 0.0f);

	glBuffer->glTexCoord2f(1.0f, 1.0f);
	glBuffer->glVertex3f(1.0f, 1.0f, 0.0f);
	
	glBuffer->glTexCoord2f(0.0f, 1.0f);
	glBuffer->glVertex3f(0.0f, 1.0f, 0.0f);
	
	glBuffer->glTexCoord2f(1.0f, 0.0f);
	glBuffer->glVertex3f(1.0f, 0.0f, 0.0f);

	glBuffer->glEnd();

	_vertexBufferID = glBuffer->GetVertexBufferID();
	_uvBufferID = glBuffer->GetUVBufferID();
	_vertexCount = glBuffer->GetVertexCount();

	if(glBuffer)
	{
		delete glBuffer;
		glBuffer = NULL;
	}
}

void Texture2D::SetBoundsByPoints(float x1, float y1, float x2, float y2)
{
	if(x1 > x2)
	{
		float temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if(y1 > y2)
	{
		float temp = y1;
		y1 = y2;
		y2 = temp;
	}

	x = x1;
	y = y1;
	w = x2-x1;
	h = y2-y1;		
}

void Texture2D::FitIntoBoundry(float bx, float by, float bw, float bh)
{
	float minX = x;
	float minY = y;
	float maxX = x+w;
	float maxY = y+h;

	if(minX < bx)		minX = bx;
	if(minY < by)		minY = by;
	if(maxX > bx+bw)	maxX = bx+bw;
	if(maxY > by+bh)	maxY = by+bh;

	if(maxX < bx)		maxX = bx;
	if(maxY < by)		maxY = by;
	if(minX > bx+bw)	minX = bx+bw;
	if(minY > by+bh)	minY = by+bh;

	x = minX;
	y = minY;
	w = maxX - minX;
	h = maxY - minY;
}

void Texture2D::Draw()
{
	glPushMatrix();

	glTranslatef(x, y, 0);
	glScalef(w, h, 0);

	_shaderProgram->Begin();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glUniform1i(glGetUniformLocation(_shaderProgram->ProgramID(), "textureID"), 0);

	glUniform1i(glGetUniformLocation(_shaderProgram->ProgramID(), "w"), (int)_textureWidth);
	glUniform1i(glGetUniformLocation(_shaderProgram->ProgramID(), "h"), (int)_textureHeight);

	GLuint uvID = glGetAttribLocation(_shaderProgram->ProgramID(), "uv");
	glEnableVertexAttribArray(uvID);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexID);
	glDisableVertexAttribArray(uvID);

	_shaderProgram->End();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}


GLuint Texture2D::LoadFile(string filePath)
{
	GLuint textureID = 0;

	if(filePath.length()>0)
	{
		ImageBuffer* imageBuffer = new ImageBuffer(filePath);

		_textureWidth = imageBuffer->GetWidth();
		_textureHeight = imageBuffer->GetHeight();

		textureID = GenerateGLTextureID(imageBuffer->GetBuffer(), imageBuffer->GetWidth(), 
										imageBuffer->GetHeight(), imageBuffer->GetBytesPerPixel());
		
		delete imageBuffer;
	}

	return textureID;
}

GLuint Texture2D::GenerateGLTextureID(void* data, int width, int height, int bytesPP)
{
	GLuint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1);

	if(bytesPP == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	return textureID;
}

Texture2D::~Texture2D()
{
	if(_shaderProgram)
	{
		delete _shaderProgram;
		_shaderProgram = NULL;
	}
}