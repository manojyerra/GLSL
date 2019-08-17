#include "GLMSFBO.h"
#include "GLMemory.h"

GLMSFBO::GLMSFBO(int w, int h)
{
	_w = w;
	_h = h;

	_msFBOID = 0;
	_msTexID = 0;
	_depthBufID = 0;
	_fboID = 0;
	_texID = 0;

	//unsigned int framebuffer;
	//glGenFramebuffers(1, &framebuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//// create a multisampled color attachment texture
	//unsigned int textureColorBufferMultiSampled;
	//glGenTextures(1, &textureColorBufferMultiSampled);
	//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
	//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, w, h, GL_TRUE);
	//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
	//// create a (also multisampled) renderbuffer object for depth and stencil attachments
	//unsigned int rbo;
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, w, h);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	throw new std::exception("Error: Failed to created MSFBO.");

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	_msFBOID = GLCreateFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, _msFBOID);
	_msTexID = GLCreateMSTexture(GL_RGB, w, h);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _msTexID, 0);
	_depthBufID = GLCreateMSRenderBuffer(w, h, GL_DEPTH_COMPONENT24);
	AttachDepthBufferToFBO(_depthBufID);

	if (!isMSFBOCreated())
		throw new std::exception("Error: Failed to created MSFBO.");

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
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _msFBOID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fboID);
	glBlitFramebuffer(0, 0, _w, _h, 0, 0, _w, _h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

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
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
	bool created = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return created;
}

bool GLMSFBO::isMSFBOCreated()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _msFBOID);
	bool created = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
