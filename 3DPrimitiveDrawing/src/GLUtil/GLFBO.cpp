#include "GLFBO.h"
#include "GLMemory.h"

GLFBO::GLFBO(int w, int h)
{
	_w = w;
	_h = h;

	_fboID = GLCreateFrameBuffer;
	BindFBO();
	_texID = CreateEmptyTexture(w, h);
	AttachTexToFBO(_texID);
	_depthBufID = CreateDepthBuffer(_w, _h);
	AttachDepthBufferToFBO(_depthBufID);

	if (!isFBOCreated())
		throw new std::exception("Error: Failed to created FBO.");
}

void GLFBO::BindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
}

void GLFBO::UnBindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int GLFBO::GetTextureID()
{
	return _texID;
}

void GLFBO::AttachTexToFBO(unsigned int texID)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
}

unsigned int GLFBO::CreateEmptyTexture(unsigned int w, unsigned int h)
{
	unsigned int texID = GLCreateTexture(GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

unsigned int GLFBO::CreateDepthBuffer(unsigned int w, unsigned int h)
{
	return GLCreateRenderBuffer(w, h, GL_DEPTH_COMPONENT24);
}

void GLFBO::AttachDepthBufferToFBO(unsigned int depthBufferID)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
}

bool GLFBO::isFBOCreated()
{
	BindFBO();
	bool created = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	UnBindFBO();
	return created;
}

unsigned int GLFBO::GetW()
{
	return _w;
}

unsigned int GLFBO::GetH()
{
	return _h;
}

GLFBO::~GLFBO()
{
	//Delete resources
	GLDeleteTexture(_texID);
	GLDeleteRenderBuffer(_depthBufID);

	//Delete frame buffer
	GLDeleteFrameBuffer(_fboID);
}