#include "GLFBO.h"
#include "GLMemory.h"

GLFBO::GLFBO(int w, int h)
{
	_w = w;
	_h = h;

	glGenFramebuffersEXT(1, &_fboID);
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
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fboID);
}

void GLFBO::UnBindFBO()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

unsigned int GLFBO::GetTextureID()
{
	return _texID;
}

void GLFBO::AttachTexToFBO(unsigned int texID)
{
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texID, 0);
}

unsigned int GLFBO::CreateDepthBuffer(int w, int h)
{
	return GLCreateRenderBuffer(w, h, GL_DEPTH_COMPONENT24);
}

void GLFBO::AttachDepthBufferToFBO(unsigned int depthBufferID)
{
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBufferID);
}

bool GLFBO::isFBOCreated()
{
	BindFBO();
	bool created = (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT);
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
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDeleteFramebuffersEXT(1, &_fboID);
}