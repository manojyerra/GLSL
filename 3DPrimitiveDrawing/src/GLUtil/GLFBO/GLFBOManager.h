#ifndef GLFBOManager_H
#define GLFBOManager_H

#include "GLFBO.h"
#include "GLMSFBO.h"
#include <vector>

class GLFBOManager
{
private:
	static GLFBOManager* _ref;

	unsigned int _sw;
	unsigned int _sh;
	GLFBO* _defaultFBO;
	GLMSFBO* _defaultMSFBO;

	GLFBOManager();
	~GLFBOManager();

public:
	static GLFBOManager* GetInstance();
	static void DeleteInstance();

	void Init(int sw, int sh);
	void SetScreenSize(int sw, int sh);

	void BindDefaultFBO();
	void UnBindDefaultFBO();
	unsigned int GetDefaultFBOTexID();

	void BindDefaultMSFBO();
	void UnBindDefaultMSFBO();
	unsigned int GetDefaultMSFBOTexID();
};

#endif
