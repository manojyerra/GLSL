#include "GLFBO.h"
#include "GLMemory.h"

GLFBO::GLFBO(int w, int h)
{
	_w = w;
	_h = h;

	glGenFramebuffers(1, &_fboID);
	BindFBO();
	_texID = GLCreateTexture(w, h, 3, NULL); //Creating empty texture
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

unsigned int GLFBO::CreateDepthBuffer(int w, int h)
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

	//Bind 0, which means render to back buffer, as a result, fb is unbound
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &_fboID);
}