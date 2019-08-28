#include "GLMeshRenderer.h"
#include "GLMemory.h"

GLMeshRenderer::GLMeshRenderer(BaseModelIO* baseModelIO, int shaderType)
{
	_bufferBuilder = new GLBufferBuilder();
	_bufferBuilder->SetVertexBuffer(baseModelIO->GetVertexBuffer(), baseModelIO->GetVertexBufferSize());
	_bufferBuilder->SetNormalBuffer(baseModelIO->GetNormalBuffer(), baseModelIO->GetNormalBufferSize());
	_bufferBuilder->SetUVBuffer(baseModelIO->GetUVBuffer(), baseModelIO->GetUVBufferSize());
	_bufferBuilder->SetColorBuffer(baseModelIO->GetColorBuffer(), baseModelIO->GetColorBufferSize());
	_bufferBuilder->SetIndicesBuffer(baseModelIO->GetIndicesBuffer(), baseModelIO->GetIndicesBufferSize());
	_bufferBuilder->SetImageBuffer(baseModelIO->GetImageBuffer());
	_bufferBuilder->build();

	_bBox = baseModelIO->GetAABB();

	_primitiveType = triangles;

	_shaderCreator = new ShaderCreator(shaderType, _bufferBuilder);
	_visible = true;
	_alpha = 1.0f;
}

void GLMeshRenderer::UpdateColorBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_bufferBuilder->UpdateColorBuffer(buffer, len);
	_shaderCreator->GetCurrentShader()->SetColorBufferID(_bufferBuilder->GetColorBufferID());
}

void GLMeshRenderer::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void GLMeshRenderer::SetPos(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
}

void GLMeshRenderer::SetRot(float xRot, float yRot, float zRot)
{
	_modelMat.SetRotation(xRot, yRot, zRot);
}

glm::vec3 GLMeshRenderer::GetPos()
{
	return glm::vec3(_modelMat.m[12], _modelMat.m[13], _modelMat.m[14]);
}

AABB GLMeshRenderer::GetAABB()
{
	return _bBox;
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

bool GLMeshRenderer::IsVisible()
{
	return _visible;
}

void GLMeshRenderer::SetPrimitiveType(int primitiveType)
{
	_primitiveType = primitiveType;
}

void GLMeshRenderer::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void GLMeshRenderer::SetVisible(bool visible)
{
	_visible = visible;
}

void GLMeshRenderer::DrawForPicking(glm::vec3 color)
{
	if (_visible && _alpha == 1.0f)
	{
		unsigned int shaderType = _shaderCreator->GetCurrentShaderType();

		_shaderCreator->SetShader(BASIC_SHADER);
		BasicShader* basicShader = (BasicShader*)_shaderCreator->GetCurrentShader();
		basicShader->SetColor(color);
		basicShader->SetModelMatrix(_modelMat.m);
		basicShader->Begin();
		basicShader->SetUniformsAndAttributes();
		glDrawArrays(_primitiveType, 0, (GLsizei)(_bufferBuilder->GetVertexBufferSize() / 12));
		basicShader->End();

		_shaderCreator->SetShader(shaderType);
	}
}

void GLMeshRenderer::DrawWireFrame()
{
	if (_visible)
	{
		Shader* shader = _shaderCreator->GetCurrentShader();

		if (shader)
		{
			unsigned int prevShaderType = GetCurrentShaderType();

			SetShader(BASIC_SHADER);
			BasicShader* basicShader = (BasicShader*)_shaderCreator->GetShader(BASIC_SHADER);

			basicShader->SetModelMatrix(_modelMat.m);
			basicShader->Begin();

			basicShader->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
			basicShader->SetUniformsAndAttributes();
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(_primitiveType, 0, (GLsizei)(_bufferBuilder->GetVertexBufferSize() / 12));

			basicShader->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
			basicShader->SetUniformsAndAttributes();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(_primitiveType, 0, (GLsizei)(_bufferBuilder->GetVertexBufferSize() / 12));

			basicShader->End();

			SetShader(prevShaderType);
		}
	}
}

void GLMeshRenderer::Draw()
{
	if (_visible)
	{
		Shader* shader = _shaderCreator->GetCurrentShader();

		if (shader)
		{
			shader->SetAlpha(_alpha);
			shader->SetModelMatrix(_modelMat.m);
			shader->Begin();
			shader->SetUniformsAndAttributes();

			unsigned int vertexCount = _bufferBuilder->GetVertexBufferSize() / 12;
			glDrawArrays(_primitiveType, 0, (GLsizei)(vertexCount));

			shader->End();
		}
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