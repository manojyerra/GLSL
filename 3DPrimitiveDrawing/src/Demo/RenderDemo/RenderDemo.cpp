#include "RenderDemo.h"
#include "Cam.h"
#include "ObjReader.h"

RenderDemo::RenderDemo(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_floor = new Floor();

	_numModels = 5;
	_selectedModel = nullptr;


	GLMeshRenderer* meshRenderer1 = new GLMeshRenderer(&ObjReader("data/teapot"), GLMeshRenderer::PBR_SHADER);
	GLMeshRenderer* meshRenderer2 = new GLMeshRenderer(&ObjReader("data/teapot"), GLMeshRenderer::PBR_SHADER);
	GLMeshRenderer* meshRenderer3 = new GLMeshRenderer(&ObjReader("data/teapot"), GLMeshRenderer::PBR_SHADER);
	GLMeshRenderer* meshRenderer4 = new GLMeshRenderer(&ObjReader("data/teapot"), GLMeshRenderer::PBR_SHADER);
	GLMeshRenderer* meshRenderer5 = new GLMeshRenderer(&ObjReader("data/teapot"), GLMeshRenderer::PBR_SHADER);

	meshRenderer1->SetPos(0.0f, 0.0f, 0.0f);
	meshRenderer2->SetPos(-3.0f, 0.0f, 0.0f);
	meshRenderer3->SetPos(3.0f, 0.0f, 0.0f);
	meshRenderer4->SetPos(-6.0f, 0.0f, 0.0f);
	meshRenderer5->SetPos(6.0f, 0.0f, 0.0f);

	for(int i=0; i<_numModels; i++)
	{
		GLMeshRenderer* meshRenderer = NULL;

		if (i == 0)			meshRenderer = meshRenderer1;
		else if (i == 1)	meshRenderer = meshRenderer2;
		else if (i == 2)	meshRenderer = meshRenderer3;
		else if (i == 3)	meshRenderer = meshRenderer4;
		else if (i == 4)	meshRenderer = meshRenderer5;

		_modelVec.push_back(meshRenderer);
	}

	_selectedModel = _modelVec[0];
	
	_shaderFrame = new ShaderFrame(0.0f, 0.0f, 300, 910);
	_shaderFrame->SetMeshRenderer(_selectedModel);

	_modelDrawingFrame = new ModelDrawingFrame(_sw - 300, 130, 300.0f, 200.0f, this);
	_applyShaderFrame = new ApplyShaderFrame(_sw - 300, 330, 300.0f, 200.0f, this);


	for (int i = 0; i < _modelVec.size(); i++)
	{
		glm::vec3 pos = _modelVec[i]->GetPos();

		_modelDrawingFrame->modelBoxVec[i]->positionModelX->SetDouble(pos.x, 3);
		_modelDrawingFrame->modelBoxVec[i]->positionModelY->SetDouble(pos.y, 3);
		_modelDrawingFrame->modelBoxVec[i]->positionModelZ->SetDouble(pos.z, 3);
	}
}

void RenderDemo::SetScreenSize(int sw, int sh)
{
	_sw = sw;
	_sh = sh;
	_modelDrawingFrame->SetPos(_sw - _modelDrawingFrame->GetWidth(), 130);
	_applyShaderFrame->SetPos(_sw - _applyShaderFrame->GetWidth(), 330);
}

void RenderDemo::Draw()
{
	glClearColor(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _sw, _sh);
	glEnable(GL_DEPTH_TEST);

	Cam::GetInstance()->SetPerspectiveProjection();
	Cam::GetInstance()->SetViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	_floor->Draw();

	for (int i = 0; i < _modelVec.size(); i++)
	{
		if (_modelDrawingFrame->modelBoxVec[i]->modelCheckBox->IsSelected())
		{
			_modelVec[i]->Draw();
		}
	}
}

void RenderDemo::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();

	if(com == _applyShaderFrame->_model)
	{
		printf("Selected Model is = %s", _applyShaderFrame->_model->GetSelectedItemName().c_str());
	}
}

void RenderDemo::SetVisibleFrames(bool visible)
{
	_shaderFrame->GetFrame()->SetVisible(visible);
	_modelDrawingFrame->GetFrame()->SetVisible(visible);
	_applyShaderFrame->GetFrame()->SetVisible(visible);
}

RenderDemo::~RenderDemo()
{
	if (_floor)
	{
		delete _floor;
		_floor = NULL;
	}

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