#ifdef _ENABLE_DEMO

#include "RenderDemo.h"
#include "Cam.h"
#include "ObjReader.h"
#include "GLState.h"
#include "STLReader.h"

RenderDemo::RenderDemo(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	_floor = new Floor();

	_numModels = 5;
	_selectedModel = nullptr;

	_meshManager = new GLMeshManager(sw, sh);

	GLMeshRenderer* meshRenderer1 = nullptr;
	GLMeshRenderer* meshRenderer2 = nullptr;
	GLMeshRenderer* meshRenderer3 = nullptr;
	GLMeshRenderer* meshRenderer4 = nullptr;
	GLMeshRenderer* meshRenderer5 = nullptr;

	if (_numModels >= 1) meshRenderer1 = _meshManager->AddMeshRenderer("data/BigSize/Plane/objFile.obj", PBR_SHADER, BaseModelIO::OBJ_MODEL);
	if (_numModels >= 2) meshRenderer2 = _meshManager->AddMeshRenderer("data/BigSize/STLCar/stlFile.stl", PBR_SHADER, BaseModelIO::STL_MODEL);
	if (_numModels >= 3) meshRenderer3 = _meshManager->AddMeshRenderer("data/BigSize/Truck/objFile.obj", PBR_SHADER, BaseModelIO::OBJ_MODEL);
	if (_numModels >= 4) meshRenderer4 = _meshManager->AddMeshRenderer("data/BigSize/STLCar/stlFile.stl", PBR_WITH_TEXTURE_SHADER, BaseModelIO::STL_MODEL);
	if (_numModels >= 5) meshRenderer5 = _meshManager->AddMeshRenderer("data/Teapot/objFile.obj", PBR_SHADER, BaseModelIO::OBJ_MODEL);

	if (meshRenderer1) meshRenderer1->SetPos(0.0f, 0.0f, -20.0f);
	if (meshRenderer2) meshRenderer2->SetPos(-8.0f, 0.0f, 0.0f);
	if (meshRenderer3) meshRenderer3->SetPos(2.0f, -3.0f, 2.0f);
	if (meshRenderer4) meshRenderer4->SetPos(0.0f, 0.0f, -12.0f);
	if (meshRenderer5) meshRenderer5->SetPos(12.0f, 0.0f, 0.0f);

	_selectedModel = meshRenderer1;

	_shaderFrame = new ShaderFrame(0.0f, 0.0f, 300.0f, 910.0f, this);
	_shaderFrame->SetMeshRenderer(_selectedModel);

	_modelVisibilityFrame = new ModelVisibilityFrame(_sw - 300.0f, 152.0f, 300.0f, 200.0f, this);
	_modelSelectionFrame = new ModelSelectionFrame(_sw - 300.0f, 353.0f, 300.0f, 200.0f, this);

	for (int i = 0; i < _modelVec.size(); i++)
	{
		glm::vec3 pos = _modelVec[i]->GetPos();

		_modelVisibilityFrame->modelBoxVec[i]->positionModelX->SetDouble(pos.x, 3);
		_modelVisibilityFrame->modelBoxVec[i]->positionModelY->SetDouble(pos.y, 3);
		_modelVisibilityFrame->modelBoxVec[i]->positionModelZ->SetDouble(pos.z, 3);
	}

	_enableSSAO = false;
	_ssao = new GLSSAO(_sw, _sh);
}

void RenderDemo::SetScreenSize(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	glViewport(0, 0, _sw, _sh);

	_modelVisibilityFrame->SetPos(_sw - _modelVisibilityFrame->GetWidth(), 152);
	_modelSelectionFrame->SetPos(_sw - _modelSelectionFrame->GetWidth(), 353);

	if (_ssao)
	{
		delete _ssao;
		_ssao = new GLSSAO(_sw, _sh);
	}
}

void RenderDemo::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam* cam = Cam::GetInstance();
	cam->SetPerspectiveProjection();
	cam->SetViewMatrix();
	cam->UpdateCamera();

	if (_enableSSAO)
	{
		bool depth = GLState::GLEnable(GL_DEPTH_TEST, true);
		bool blend = GLState::GLEnable(GL_BLEND, false);
		bool cullFace = GLState::GLEnable(GL_CULL_FACE, true);

		_ssao->Begin();
		DrawObjectsForSSAO();
		_ssao->End();

		GLState::GLEnable(GL_DEPTH_TEST, depth);
		GLState::GLEnable(GL_BLEND, blend);
		GLState::GLEnable(GL_CULL_FACE, cullFace);
	}

	_floor->Draw();

	for (int i = 0; i < _modelVec.size(); i++)
	{
		if (_modelVisibilityFrame->modelBoxVec[i]->modelCheckBox->IsSelected())
		{
			_modelVec[i]->Draw();
		}
	}

	if (_enableSSAO)
	{
		bool blend = GLState::GLEnable(GL_BLEND, true);
		bool cullFace = GLState::GLEnable(GL_CULL_FACE, false);
		bool depth = GLState::GLEnable(GL_DEPTH_TEST, false);
		
		_ssao->DrawOcclusionMap();

		GLState::GLEnable(GL_DEPTH_TEST, depth);
		GLState::GLEnable(GL_CULL_FACE, cullFace);
		GLState::GLEnable(GL_BLEND, blend);
	}
}

void RenderDemo::DrawObjectsForSSAO()
{
	for (int i = 0; i < _modelVec.size(); i++)
	{
		if (_modelVisibilityFrame->modelBoxVec[i]->modelCheckBox->IsSelected())
		{
			unsigned int shaderType = _modelVec[i]->GetCurrentShaderType();
			_modelVec[i]->SetShader(SSAO_GEOMETRY_PASS_SHADER);
			_modelVec[i]->Draw();
			_modelVec[i]->SetShader(shaderType);
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
				_modelVec[i]->SetShader(PBR_SHADER);
			}
		}
		else
		{
			for (int i = 0; i < _modelVec.size(); i++)
			{
				_modelVec[i]->SetShader(PHONG_PER_VERTEX_SHADER);
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
				float x = (float)box->positionModelX->GetDouble();
				float y = (float)box->positionModelY->GetDouble();
				float z = (float)box->positionModelZ->GetDouble();

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

void RenderDemo::SetEnableSSAO(bool enable)
{
	_enableSSAO = enable;
}

RenderDemo::~RenderDemo()
{
	if (_ssao)
	{
		delete _ssao;
		_ssao = nullptr;
	}

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

#endif