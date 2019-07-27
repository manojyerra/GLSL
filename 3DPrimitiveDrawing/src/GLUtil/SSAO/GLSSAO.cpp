#include "GLSSAO.h"
#include "GLBatch.h"

GLSSAO::GLSSAO(float w, float h) : GLSSAOBufferBuilder(w, h)
{
	_quadRenderer = nullptr;

	_modelMat.m[0] = w;
	_modelMat.m[5] = h;
	_modelMat.m[12] = 0;
	_modelMat.m[13] = 0;

	ModelInfo modelInfo = CreateModeInfo();

	_quadRenderer = new GLMeshRenderer(&modelInfo, GLMeshRenderer::SSAO_SHADER);
	_quadRenderer->SetPrimitiveType(GLMeshRenderer::triangleStrip);

	SSAOShader* ssaoShader = (SSAOShader*)_quadRenderer->GetCurrentShader();
	ssaoShader->SetGPositionTexID(GetGPositionTexID());
	ssaoShader->SetGNormalTexID(GetGNormalTexID());
	ssaoShader->SetNoiseTexID(GetNoiseTexID());
	ssaoShader->SetSamples(GetSamples());
}

ModelInfo GLSSAO::CreateModeInfo()
{
	GLBatch buffer(100, false, true, false);

	buffer.glBegin();

	buffer.glTexCoord2f(0, 1);
	buffer.glVertex3f(0, 0, 0);

	buffer.glTexCoord2f(1, 1);
	buffer.glVertex3f(1, 0, 0);

	buffer.glTexCoord2f(0, 0);
	buffer.glVertex3f(0, 1, 0);

	buffer.glTexCoord2f(1, 0);
	buffer.glVertex3f(1, 1, 0);

	ModelInfo modelInfo;
	modelInfo.SetVertexBuffer(buffer.GetVertexBuffer(), buffer.GetVertexBufferSize());
	modelInfo.SetUVBuffer(buffer.GetUVBuffer(), buffer.GetUVBufferSize());

	return modelInfo;
}

void GLSSAO::Draw()
{
	_quadRenderer->SetModelMatrix(_modelMat.m);
	_quadRenderer->Draw();
}

GLSSAO::~GLSSAO()
{
	if (_quadRenderer)
	{
		delete _quadRenderer;
		_quadRenderer = NULL;
	}
}


//void GLSSAO::SetBounds(float x, float y, float w, float h)
//{
//	_modelMat.m[0] = w;
//	_modelMat.m[5] = h;
//
//	_modelMat.m[12] = x;
//	_modelMat.m[13] = y;
//}