#ifndef GLMeshRenderer_H
#define GLMeshRenderer_H

#include "GLMat.h"
#include "ImageBuffer.h"
#include "../Common/IO/ModelIO/ObjReader.h"

class GLMeshRenderer
{
private:

public:
	GLMeshRenderer(ObjReader* reader);
	~GLMeshRenderer();	
};

#endif