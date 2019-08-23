#ifndef GLMSFBO_H
#define GLMSFBO_H

class GLMSFBO
{
private:
	unsigned int _msFBOID;
	unsigned int _msTexID;

	unsigned int _fboID;
	unsigned int _texID;
	unsigned int _depthBufID;

	unsigned int _w;
	unsigned int _h;

	unsigned int _numSamples;


	void AttachDepthBufferToFBO(unsigned int depthBufferID);
	unsigned int CreateEmptyTexture(unsigned int w, unsigned int h);

public:
	GLMSFBO(int w, int h);
	~GLMSFBO();

	void BindFBO();
	void UnBindFBO();
	unsigned int GetTextureID();
	bool isFBOCreated();
	bool isMSFBOCreated();

	unsigned int GetW();
	unsigned int GetH();
};

#endif
