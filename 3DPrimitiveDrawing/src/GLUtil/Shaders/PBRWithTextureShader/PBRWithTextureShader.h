#ifndef PBRWithTextureShader_H
#define PBRWithTextureShader_H

#include "PBRShader.h"

class PBRWithTextureShader : public PBRShader
{
private:
	unsigned int _uvBufferID;
	unsigned int _baseTexID;

public:
	PBRWithTextureShader();
	~PBRWithTextureShader();

	void SetUVBufferID(unsigned int bufferID);
	void SetBaseTexID(unsigned int texID);

	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif