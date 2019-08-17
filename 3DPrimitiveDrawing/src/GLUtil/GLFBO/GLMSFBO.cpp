#include "GLMSFBO.h"
#include "GLMemory.h"

GLMSFBO::GLMSFBO(int w, int h)
{
	_w = w;
	_h = h;

	_msFBOID = GLCreateFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, _msFBOID);
	_msTexID = GLCreateMSTexture(GL_RGB, w, h, GL_RGB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _msTexID, 0);
	_depthBufID = GLCreateRenderBuffer(w, h, GL_DEPTH_COMPONENT24);
	AttachDepthBufferToFBO(_depthBufID);

	_fboID = GLCreateFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
	_texID = CreateEmptyTexture(w, h);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texID, 0);

	if (!isFBOCreated())
		throw new std::exception("Error: Failed to created FBO.");
}

void GLMSFBO::BindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _msFBOID);
}

void GLMSFBO::UnBindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int GLMSFBO::GetTextureID()
{
	return _texID;
}

unsigned int GLMSFBO::CreateEmptyTexture(unsigned int w, unsigned int h)
{
	unsigned int texID = GLCreateTexture(GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void GLMSFBO::AttachDepthBufferToFBO(unsigned int depthBufferID)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
}

bool GLMSFBO::isFBOCreated()
{
	BindFBO();
	bool created = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	UnBindFBO();
	return created;
}

unsigned int GLMSFBO::GetW()
{
	return _w;
}

unsigned int GLMSFBO::GetH()
{
	return _h;
}

GLMSFBO::~GLMSFBO()
{
	//Delete resources
	GLDeleteTexture(_msTexID);
	GLDeleteTexture(_texID);
	GLDeleteRenderBuffer(_depthBufID);

	//Delete frame buffers
	GLDeleteFrameBuffer(_fboID);
	GLDeleteFrameBuffer(_msFBOID);
}
