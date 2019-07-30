#include "GLSSAOBufferBuilder.h"
#include "GLInclude.h"
#include <glm/glm.hpp>
#include "GLMemory.h"

GLSSAOBufferBuilder::GLSSAOBufferBuilder(int w, int h)
{
	_w = w;
	_h = h;

	_gBufferFBO = 0;

	_gPosition = 0;
	_gNormal = 0;
	_gAlbedo = 0;
	_gDepthRenderBufferID = 0;

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
	unsigned int gBufferFBO = GLCreateFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);

	unsigned int gPosition = GLCreateTexture(GL_RGB16F, w, h, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	unsigned int gNormal = GLCreateTexture(GL_RGB16F, w, h, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	unsigned int gAlbedo = GLCreateTexture(GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	unsigned int depthRenderBufferID = GLCreateRenderBuffer(w, h, GL_DEPTH_COMPONENT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw new std::exception("Exception : Failed to create the frame buffer.");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_gBufferFBO = gBufferFBO;
	_gPosition = gPosition;
	_gNormal = gNormal;
	_gAlbedo = gAlbedo;
	_gDepthRenderBufferID = depthRenderBufferID;
}

void GLSSAOBufferBuilder::CreateSSAOFBO(unsigned int w, unsigned int h)
{
	// SSAO color buffer
	unsigned int ssaoFBO = GLCreateFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	unsigned int ssaoColorBuffer = GLCreateTexture(GL_RED, w, h, GL_RGB, GL_FLOAT, NULL);
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
	unsigned int ssaoBlurFBO = GLCreateFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

	unsigned int ssaoColorBufferBlur = GLCreateTexture(GL_RED, _w, _h, GL_RGB, GL_FLOAT, NULL);
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
	//Delete GBuffer frame buffer
	GLDeleteTexture(_gPosition);
	GLDeleteTexture(_gNormal);
	GLDeleteTexture(_gAlbedo);
	GLDeleteRenderBuffer(_gDepthRenderBufferID);
	GLDeleteFrameBuffer(_gBufferFBO);

	//Delete SSAO frame buffer
	GLDeleteTexture(_ssaoColorAttachmentID);
	GLDeleteFrameBuffer(_ssaoFBO);

	//Delete SSAO Blur frame buffer
	GLDeleteTexture(_ssaoBlurColorAttachmentID);
	GLDeleteFrameBuffer(_ssaoBlurFBO);
}