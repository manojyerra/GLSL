#include "GLMeshManager.h"
#include "ObjReader.h"
#include "STLReader.h"

GLMeshManager::GLMeshManager(float sw, float sh)
{
	_modelVec.clear();
	_fbo = new GLFBO(sw, sh);
}

void GLMeshManager::SetScreenSize(float sw, float sh)
{
	if(_fbo)
	{
		delete _fbo;
		_fbo = new GLFBO(sw, sh);
	}
}

unsigned int GLMeshManager::Size()
{
	return _modelVec.size();
}

GLMeshRenderer* GLMeshManager::AddMeshRenderer(std::string path, unsigned int shaderType, unsigned int modelType)
{
	GLMeshRenderer* meshRenderer = nullptr;

	if (modelType == BaseModelIO::OBJ_MODEL)
	{
		ObjReader reader(path);
		meshRenderer = new GLMeshRenderer(&reader, shaderType);
	}
	else if (modelType == BaseModelIO::STL_MODEL)
	{
		STLReader reader(path);
		meshRenderer = new GLMeshRenderer(&reader, shaderType);
	}

	return meshRenderer;
}

int GLMeshManager::GetModelIndexByMousePos(float x, float y, float sw, float sh)
{
	_fbo->BindFBO();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i=0; i<(int)_modelVec.size(); i++)
	{
		float r = (i % 256) * 1.0f / 255.0f;
		float g = (i / 256) * 1.0f / 255.0f;
		float b = (i / (256*256)) * 1.0f / (255.0f);

		_modelVec[i]->DrawForPicking(glm::vec3(r, g, b));
	}



	_fbo->UnBindFBO();
}

GLMeshManager::~GLMeshManager()
{
	
}