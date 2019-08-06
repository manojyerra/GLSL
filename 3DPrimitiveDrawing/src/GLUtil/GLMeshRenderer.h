#ifndef GLMeshRenderer_H
#define GLMeshRenderer_H

#include "GLMat.h"
#include "BaseModelIO.h"
#include "GLBufferBuilder.h"
#include "AABB.h"
#include "ShaderCreator.h"

class GLMeshRenderer
{
private:
	int _primitiveType;

	GLMat _modelMat;
	AABB _bBox;
	ShaderCreator* _shaderCreator;
	bool _visible;

public:
	GLBufferBuilder* _bufferBuilder;

	enum {
		points = GL_POINTS,
		lines = GL_LINES,
		lineLoop = GL_LINE_LOOP,
		lineStrip = GL_LINE_STRIP,
		triangles = GL_TRIANGLES,
		triangleStrip = GL_TRIANGLE_STRIP,
		triangleFan = GL_TRIANGLE_FAN
	};

	GLMeshRenderer(BaseModelIO* baseModelIO, int shaderType);
	~GLMeshRenderer();

	void UpdateColorBuffer(const GLvoid* buffer, GLsizeiptr len);

	void SetModelMatrix(float* mat);
	void SetPos(float x, float y, float z);
	glm::vec3 GetPos();

	void SetShader(int shaderType);
	Shader* GetShader(int shaderType);
	Shader* GetCurrentShader();
	unsigned int GetCurrentShaderType();

	void SetPrimitiveType(int primitiveType);
	void SetAlpha(float alpha);
	void SetVisible(bool visible);
	void DrawForPicking(glm::vec3 color);
	void DrawWireFrame();
	void Draw();
};

#endif