#include "GLMeshManager.h"
#include "ObjReader.h"
#include "STLReader.h"
#include "STLReaderWithThreads.h"
#include "GLFBOManager.h"

GLMeshManager::GLMeshManager(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	_modelVec.clear();
}

void GLMeshManager::SetScreenSize(float sw, float sh)
{
	_sw = sw;
	_sh = sh;
}

unsigned int GLMeshManager::Size()
{
	return _modelVec.size();
}

GLMeshRenderer* GLMeshManager::Get(unsigned int index)
{
	if (index >= 0 && index < _modelVec.size())
		return _modelVec[index];

	return nullptr;
}

GLMeshRenderer* GLMeshManager::AddMeshRenderer(std::string path, unsigned int shaderType, unsigned int modelType)
{
	GLMeshRenderer* meshRenderer = nullptr;

	if (modelType == BaseModelIO::OBJ_MODEL)
	{
		ObjReader reader(path);
		meshRenderer = new GLMeshRenderer(&reader, shaderType);
		_modelVec.push_back(meshRenderer);
	}
	else if (modelType == BaseModelIO::STL_MODEL)
	{
		STLReader reader(path);
		meshRenderer = new GLMeshRenderer(&reader, shaderType);
		_modelVec.push_back(meshRenderer);
	}
	else if (modelType == BaseModelIO::STL_MODEL_WITH_THREADS)
	{
		STLReaderWithThreads reader(path);
		meshRenderer = new GLMeshRenderer(&reader, shaderType);
		_modelVec.push_back(meshRenderer);
	}

	return meshRenderer;
}

int GLMeshManager::GetModelIndexByMousePos(float x, float y)
{
	GLFBOManager::GetInstance()->BindDefaultFBO();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i=0; i<(int)_modelVec.size(); i++)
	{
		float r = (i % 256) * 1.0f / 255.0f;
		float g = (i / 256) * 1.0f / 255.0f;
		float b = (i / (256*256)) * 1.0f / (255.0f);

		_modelVec[i]->DrawForPicking(glm::vec3(r, g, b));
	}

	GLubyte data[4];
	glReadPixels((GLint)x, _sh - (GLint)y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

	unsigned int colorVal = (unsigned int)((data[2] << 16) + (data[1] << 8) + data[0] );

	GLFBOManager::GetInstance()->UnBindDefaultFBO();

	return (colorVal < Size()) ? colorVal : -1;
}

GLMeshManager::~GLMeshManager()
{
	for (int i = 0; i < _modelVec.size(); i++)
	{
		GLMeshRenderer* model = _modelVec[i];

		if (model)
		{
			delete model;
			model = NULL;
		}
	}
}