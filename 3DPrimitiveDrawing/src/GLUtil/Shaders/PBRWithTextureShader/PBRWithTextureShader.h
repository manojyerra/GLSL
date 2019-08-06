#ifndef PBRWithTextureShader_H
#define PBRWithTextureShader_H

#include "PBRShader.h"

class PBRWithTextureShader : public PBRShader
{
private:

public:
	PBRWithTextureShader();
	~PBRWithTextureShader();

	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif