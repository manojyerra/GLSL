#ifndef PBRColorShader_H
#define PBRColorShader_H

#include "PBRShader.h"

class PBRColorShader : public PBRShader
{
private:

public:
	PBRColorShader();
	~PBRColorShader();

	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif