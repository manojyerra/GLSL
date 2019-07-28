#include "GLSSAOBufferBuilder.h"
#include "GLInclude.h"
#include <glm/glm.hpp>

GLSSAOBufferBuilder::GLSSAOBufferBuilder(int w, int h)
{
	_w = w;
	_h = h;

	_gBufferFBO = 0;

	_gPosition = 0;
	_gNormal = 0;
	_gAlbedo = 0;

	_ssaoFBO = 0;
	_ssaoColorAttachmentID = 0;
	
	_ssaoBlurFBO = 0;
	_ssaoBlurColorAttachmentID = 0;

	CreateGBufferFBO(_w, _h);
	CreateSSAOFBO(_w, _h);
	CreateSSAOBlurFBO(_w, _h);
}

void GLSSAOBufferBuilder::CreateGBufferFBO(unsigned int w, unsigned int h)
{
	unsigned int gBufferFBO;
	glGenFramebuffers(1, &gBufferFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);

	// position color buffer
	unsigned int gPosition = 0;
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	unsigned int gNormal = 0;
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// color + specular color buffer	
	unsigned int gAlbedo = 0;
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw new std::exception("Framebuffer not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_gBufferFBO = gBufferFBO;

	_gPosition = gPosition;
	_gNormal = gNormal;
	_gAlbedo = gAlbedo;
}

void GLSSAOBufferBuilder::CreateSSAOFBO(unsigned int w, unsigned int h)
{
	// SSAO color buffer
	unsigned int ssaoFBO;
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	unsigned int ssaoColorBuffer;
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw new std::exception("SSAO Framebuffer not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_ssaoFBO = ssaoFBO;
	_ssaoColorAttachmentID = ssaoColorBuffer;
}

void GLSSAOBufferBuilder::CreateSSAOBlurFBO(unsigned int w, unsigned int h)
{
	unsigned int ssaoBlurFBO;
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

	unsigned int ssaoColorBufferBlur;
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _w, _h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw new std::exception("SSAO Blur Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_ssaoBlurFBO = ssaoBlurFBO;
	_ssaoBlurColorAttachmentID = ssaoColorBufferBlur;
}

unsigned int GLSSAOBufferBuilder::GetGBufferFBO()
{
	return _gBufferFBO;
}

unsigned int GLSSAOBufferBuilder::GetSSAOFBO()
{
	return _ssaoFBO;
}

unsigned int GLSSAOBufferBuilder::GetSSAOColorAttachmentID()
{
	return _ssaoColorAttachmentID;
}

unsigned int GLSSAOBufferBuilder::GetSSAOBlurFBO()
{
	return _ssaoBlurFBO;
}

unsigned int GLSSAOBufferBuilder::GetSSAOBlurColorAttachmentID()
{
	return _ssaoBlurColorAttachmentID;
}

unsigned int GLSSAOBufferBuilder::GetGPositionTexID()
{
	return _gPosition;
}

unsigned int GLSSAOBufferBuilder::GetGNormalTexID()
{
	return _gNormal;
}

unsigned int GLSSAOBufferBuilder::GetGAlbedoTexID()
{
	return _gAlbedo;
}

unsigned int GLSSAOBufferBuilder::GetW()
{
	return _w;
}

unsigned int GLSSAOBufferBuilder::GetH()
{
	return _h;
}

GLSSAOBufferBuilder::~GLSSAOBufferBuilder()
{
	////Delete resources
	//glDeleteTextures(1, &_texID);
	//glDeleteRenderbuffersEXT(1, &_depthBufID);
	////Bind 0, which means render to back buffer, as a result, fb is unbound
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glDeleteFramebuffersEXT(1, &_fboID);
}