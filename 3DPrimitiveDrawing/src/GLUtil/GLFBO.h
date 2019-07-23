#ifndef GLFBO_H
#define GLFBO_H

class GLFBO
{
private:
	unsigned int _fboID;
	unsigned int _texID;
	unsigned int _depthBufID;

	unsigned int _w;
	unsigned int _h;

	void AttachTexToFBO(unsigned int texID);
	unsigned int CreateDepthBuffer(int w, int h);
	void AttachDepthBufferToFBO(unsigned int depthBufferID);

public:
	GLFBO(int w, int h);
	~GLFBO();

	void BindFBO();
	void UnBindFBO();
	unsigned int GetTextureID();
	bool isFBOCreated();

	unsigned int GetW();
	unsigned int GetH();
};

#endif
