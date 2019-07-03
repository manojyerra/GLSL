#ifndef GBufferFBO_H
#define GBufferFBO_H

#include "Macros.h"

class GBufferFBO
{
private:
	unsigned int _w;
	unsigned int _h;

	unsigned int _gBuffer;

	unsigned int _rboDepth;

	unsigned int _ssaoFBO;
	unsigned int _ssaoBlurFBO;


public:
	GBufferFBO(int w, int h);
	~GBufferFBO();

	unsigned int GetW();
	unsigned int GetH();
};

#endif
