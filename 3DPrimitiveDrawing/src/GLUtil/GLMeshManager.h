#ifndef GLMeshManager_H
#define GLMeshManager_H

#include "GLMeshRenderer.h"

class GLMeshManager
{
private:

public:
	GLMeshManager();
	~GLMeshManager();

	GLMeshRenderer* AddMeshRenderer(std::string path, unsigned int shaderType);

};

#endif
