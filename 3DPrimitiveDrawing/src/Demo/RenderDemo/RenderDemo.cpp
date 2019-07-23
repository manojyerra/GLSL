#include "RenderDemo.h"
#include "Cam.h"
#include "ObjReader.h"

RenderDemo::RenderDemo(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_floor = new Floor();

	_numModels = 1;
	_selectedModel = nullptr;

	GLMeshRenderer* meshRenderer1 = nullptr;
	GLMeshRenderer* meshRenderer2 = nullptr;
	GLMeshRenderer* meshRenderer3 = nullptr;
	GLMeshRenderer* meshRenderer4 = nullptr;
	GLMeshRenderer* meshRenderer5 = nullptr;

	if (_numModels >= 1) meshRenderer1 = new GLMeshRenderer(&ObjReader("data/demo/Teapot"), GLMeshRenderer::PBR_SHADER);
	if (_numModels >= 2) meshRenderer2 = new GLMeshRenderer(&ObjReader("data/demo/Trike"), GLMeshRenderer::PBR_SHADER);
	if (_numModels >= 3) meshRenderer3 = new GLMeshRenderer(&ObjReader("data/demo/Truck"), GLMeshRenderer::PBR_SHADER);
	if (_numModels >= 4) meshRenderer4 = new GLMeshRenderer(&ObjReader("data/demo/Plane"), GLMeshRenderer::PBR_WITH_TEXTURE_SHADER);
	if (_numModels >= 5) meshRenderer5 = new GLMeshRenderer(&ObjReader("data/demo/Teapot"), GLMeshRenderer::PBR_SHADER);

	if (meshRenderer1) meshRenderer1->SetPos(-4.0f, -3.0f, 1.5f);
	if (meshRenderer2) meshRenderer2->SetPos(-8.0f, 0.0f, 0.0f);
	if (meshRenderer3) meshRenderer3->SetPos(2.0f, -3.0f, 2.0f);
	if (meshRenderer4) meshRenderer4->SetPos(0.0f, 0.0f, -12.0f);
	if (meshRenderer5) meshRenderer5->SetPos(12.0f, 0.0f, 0.0f);

	for (int i = 0; i < _numModels; i++)
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

	_shaderFrame = new ShaderFrame(0.0f, 0.0f, 300, 910, this);
	_shaderFrame->SetMeshRenderer(_selectedModel);

	_modelVisibilityFrame = new ModelVisibilityFrame(_sw - 300, 152, 300.0f, 200.0f, this);
	_modelSelectionFrame = new ModelSelectionFrame(_sw - 300, 353, 300.0f, 200.0f, this);

	for (int i = 0; i < _modelVec.size(); i++)
	{
		glm::vec3 pos = _modelVec[i]->GetPos();

		_modelVisibilityFrame->modelBoxVec[i]->positionModelX->SetDouble(pos.x, 3);
		_modelVisibilityFrame->modelBoxVec[i]->positionModelY->SetDouble(pos.y, 3);
		_modelVisibilityFrame->modelBoxVec[i]->positionModelZ->SetDouble(pos.z, 3);
	}
}

void RenderDemo::SetScreenSize(int sw, int sh)
{
	_sw = sw;
	_sh = sh;
	_modelVisibilityFrame->SetPos(_sw - _modelVisibilityFrame->GetWidth(), 152);
	_modelSelectionFrame->SetPos(_sw - _modelSelectionFrame->GetWidth(), 353);
}

void RenderDemo::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float clearValue = 100.0f/255.0f;
	glClearColor(clearValue, clearValue, clearValue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _sw, _sh);
	glEnable(GL_DEPTH_TEST);

	Cam::GetInstance()->SetPerspectiveProjection();
	Cam::GetInstance()->SetViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	_floor->Draw();

	for (int i = 0; i < _modelVec.size(); i++)
	{
		if (_modelVisibilityFrame->modelBoxVec[i]->modelCheckBox->IsSelected())
		{
			_modelVec[i]->Draw();
		}
	}
}

void RenderDemo::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();

	if (com == _modelSelectionFrame->model)
	{
		int selIndex = _modelSelectionFrame->model->GetSelectedIndex();
		_shaderFrame->SetMeshRenderer(_modelVec[selIndex]);
	}
	else if(com == _shaderFrame->shaderType && _modelSelectionFrame->applyShaderToAll->IsSelected())
	{
		if(_shaderFrame->shaderType->GetSelectedIndex() == 0)
		{
			for(int i=0; i< _modelVec.size(); i++)
			{
				_modelVec[i]->SetShader(GLMeshRenderer::PBR_SHADER);
			}
		}
		else
		{
			for (int i = 0; i < _modelVec.size(); i++)
			{
				_modelVec[i]->SetShader(GLMeshRenderer::PHONG_PER_VERTEX_SHADER);
			}
		}
	}
	else
	{
		for (int i = 0; i < _modelVec.size(); i++)
		{
			ModelBox* box = _modelVisibilityFrame->modelBoxVec[i];

			if(com == box->positionModelX || com == box->positionModelY || com == box->positionModelZ)
			{
				float x = box->positionModelX->GetDouble();
				float y = box->positionModelY->GetDouble();
				float z = box->positionModelZ->GetDouble();

				_modelVec[i]->SetPos(x, y, z);
				break;
			}
		}
	}
}

void RenderDemo::SetVisibleFrames(bool visible)
{
	_shaderFrame->GetFrame()->SetVisible(visible);
	_modelVisibilityFrame->GetFrame()->SetVisible(visible);
	_modelSelectionFrame->GetFrame()->SetVisible(visible);
}

void RenderDemo::SetFloorVisible(bool visible)
{
	_floor->SetVisible(visible);
}

Floor* RenderDemo::GetFloor()
{
	return _floor;
}

RenderDemo::~RenderDemo()
{
	if (_floor)
	{
		delete _floor;
		_floor = NULL;
	}

	if(_shaderFrame)
	{
		delete _shaderFrame;
		_shaderFrame = NULL;
	}

	if(_modelVisibilityFrame)
	{
		delete _modelVisibilityFrame;
		_modelVisibilityFrame = NULL;
	}

	if(_modelSelectionFrame)
	{
		delete _modelSelectionFrame;
		_modelSelectionFrame = NULL;
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