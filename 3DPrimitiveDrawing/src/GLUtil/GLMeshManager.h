#ifndef GLMeshManager_H
#define GLMeshManager_H

#include "GLMeshRenderer.h"
#include "GLFBO.h"

class GLMeshManager
{
private:
	vector<GLMeshRenderer*> _modelVec;
	float _sw;
	float _sh;

public:
	GLMeshManager(float sw, float sh);
	~GLMeshManager();

	void SetScreenSize(float sw, float sh);
	GLMeshRenderer* Get(unsigned int index);
	unsigned int Size();

	GLMeshRenderer* AddMeshRenderer(std::string path, unsigned int shaderType, unsigned int modelType);
	int GetModelIndexByMousePos(float x, float y);
};

#endif
