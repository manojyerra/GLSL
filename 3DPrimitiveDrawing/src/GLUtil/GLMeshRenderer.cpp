#include "GLMeshRenderer.h"
#include "GLMemory.h"

GLMeshRenderer::GLMeshRenderer(BaseModelIO* BaseModelIO, int shaderType)
{
	_bufferBuilder = new GLBufferBuilder();
	_bufferBuilder->SetVertexBuffer(BaseModelIO->GetVertexBuffer(), BaseModelIO->GetVertexBufferSize());
	_bufferBuilder->SetNormalBuffer(BaseModelIO->GetNormalBuffer(), BaseModelIO->GetNormalBufferSize());
	_bufferBuilder->SetUVBuffer(BaseModelIO->GetUVBuffer(), BaseModelIO->GetUVBufferSize());
	_bufferBuilder->SetColorBuffer(BaseModelIO->GetColorBuffer(), BaseModelIO->GetColorBufferSize());
	_bufferBuilder->SetIndicesBuffer(BaseModelIO->GetIndicesBuffer(), BaseModelIO->GetIndicesBufferSize());
	_bufferBuilder->SetImageBuffer(BaseModelIO->GetImageBuffer());
	_bufferBuilder->build();

	_bBox = BaseModelIO->GetAABB();
	_bBox.print();

	_primitiveType = triangles;

	_shaderCreator = new ShaderCreator(shaderType, _bufferBuilder);
}

void GLMeshRenderer::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void GLMeshRenderer::SetPos(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
}

glm::vec3 GLMeshRenderer::GetPos()
{
	return glm::vec3(_modelMat.m[12], _modelMat.m[13], _modelMat.m[14]);
}

void GLMeshRenderer::SetShader(int shaderType)
{
	_shaderCreator->SetShader(shaderType);
}

Shader* GLMeshRenderer::GetShader(int shaderType)
{
	return _shaderCreator->GetShader(shaderType);
}

Shader* GLMeshRenderer::GetCurrentShader()
{
	return _shaderCreator->GetCurrentShader();
}

unsigned int GLMeshRenderer::GetCurrentShaderType()
{
	return _shaderCreator->GetCurrentShaderType();
}

void GLMeshRenderer::SetPrimitiveType(int primitiveType)
{
	_primitiveType = primitiveType;
}

void GLMeshRenderer::SetAlpha(float alpha)
{
	_shaderCreator->GetCurrentShader()->SetAlpha(alpha);
}

void GLMeshRenderer::Draw()
{
	Shader* shader = _shaderCreator->GetCurrentShader();

	if (shader)
	{
		//SetShader(BASIC_SHADER);

		shader->SetModelMatrix(_modelMat.m);
		shader->Begin();

		//_basicShader->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
		//_shader->SetUniformsAndAttributes();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawArrays(_primitiveType, 0, (GLsizei)(_bufferBuilder->GetVertexBufferSize() / 12));

		//_basicShader->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
		shader->SetUniformsAndAttributes();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(_primitiveType, 0, (GLsizei)(_bufferBuilder->GetVertexBufferSize() / 12));

		shader->End();
	}
}

GLMeshRenderer::~GLMeshRenderer()
{
	if (_bufferBuilder)
	{
		delete _bufferBuilder;
		_bufferBuilder = nullptr;
	}

	if(_shaderCreator)
	{
		delete _shaderCreator;
		_shaderCreator = nullptr;
	}
}