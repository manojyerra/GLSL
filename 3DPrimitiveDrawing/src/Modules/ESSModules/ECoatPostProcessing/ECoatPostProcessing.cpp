#include "ECoatPostProcessing.h"
#include "Cam.h"
#include "Cam2D.h"
#include "ParticleRenderer.h"
#include "ECoatResultReader.h"
#include "GLFBOManager.h"
#include "Input.h"
#include "ContourMap.h"
#include "ObjReader.h"
#include "STLReader.h"
#include "STLReaderWithThreads.h"
#include "GLState.h"
#include "StringUtils.h"
#include "GLFont.h"

ECoatPostProcessing::ECoatPostProcessing(unsigned int sw, unsigned int sh, int argc, char** argv)
{
	_sw = sw;
	_sh = sh;

	//Begin: Default initialization 
	_floor = nullptr;
	_meshManager = nullptr;
	_carBody = nullptr;
	_assetsBuilder = nullptr;
	_particleMgr = nullptr;
	_drawAnodes = true;
	//End: Default initialization 

	float zNear = 0.15f;
	float zFar = 1000000.0f;
	float zNearPlaneW = 0.25f;

	Platform::SetWindowTitle("ECoat Post Processing");

	Cam::GetInstance()->Init(_sw, _sh, zNear, zFar, zNearPlaneW);
	Cam::GetInstance()->SetEnableZUpRot(true);
	Cam2D::GetInstance()->Init(_sw, _sh);

	SetGLStates();

	_floor = new Floor();
	_floor->SetRotation(90.0f, 0.0f, 0.0f);

	_meshManager = new GLMeshManager(_sw, _sh);

	_assetsBuilder = new ECoatAssetsBuilder(&ECoatAssetsReader("AdvancedRenderer_JLR_ECoating.json"), _meshManager);
	_colorBar = new ColorBar(_sw, _sh);
	_timeLineFrame = new ECoatUI(0, 0, 330, 700, _assetsBuilder->GetResultReader()->GetTotalFrameCount(), this);

	_particleMgr = new ECoatParticleDataMgr(_sw, _sh, _assetsBuilder, _colorBar);

	_texture = new GLTexture(0.0f, 0.0f, _sw, _sh);
	_needAllParticlesDraw = true;

	_showThickness = false;
	_selectedThickness = 0.0;

	SetScreenSize(_sw, _sh);
}

void ECoatPostProcessing::SetScreenSize(unsigned int sw, unsigned int sh)
{
	_sw = sw;
	_sh = sh;

	glViewport(0, 0, _sw, _sh);

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
	Cam2D::GetInstance()->SetScreenSize(_sw, _sh);

	if(_timeLineFrame)
		_timeLineFrame->SetPos(0.0f, 0.0f);

	_colorBar->OnSizeChange(sw, sh);
	_particleMgr->OnSizeChange(sw, sh);

	_needAllParticlesDraw = true;
}

void ECoatPostProcessing::SetGLStates()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ECoatPostProcessing::OnModuleChange(unsigned int sw, unsigned int sh)
{
}

void ECoatPostProcessing::Update(float deltaTime)
{
}

void ECoatPostProcessing::Draw()
{
	Cam* cam = Cam::GetInstance();
	cam->SetPerspectiveProjection();
	cam->SetViewMatrix();
	cam->UpdateCamera();

	if(Input::IsKeyPressed(Input::KEY_SPACE) || Input::IsKeyReleased(Input::KEY_SPACE))
	{
		_needAllParticlesDraw = true;
	}

	if ((Input::IsKeyPressed(Input::KEY_LEFT_ALT) || Input::IsKeyPressed(Input::KEY_RIGHT_ALT)) && Input::IsMouseClicked())
	{
		unsigned char color[4];
		bool clickedOnCarBody = _particleMgr->GetParticleColor(Input::MX, Input::MY, color);

		if (clickedOnCarBody)
		{
			_selectedThickness = _colorBar->GetValue(color[0], color[1], color[2]);

			std::string thicknessStr = StringUtils::doubleToScientificStr(_selectedThickness);
			printf("\n clicked on carbody : %d, %d, %d, Thickness = %s ", color[0], color[1], color[2], thicknessStr.c_str());
			
			_showThickness = true;
			_posForThickness = glm::ivec2(Input::MX, Input::MY);
			_needAllParticlesDraw = true;
		}
		else
		{
			printf("\n Not clicked on carbody...");
		}
	}
	else if (Input::IsMouseClicked())
	{
		_showThickness = false;
	}

	if (cam->IsCameraUpdated() || _needAllParticlesDraw)
	{
		GLFBOManager::GetInstance()->BindDefaultMSFBO();

		glClearColor(0.3,0.3,0.35,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(cam->IsCameraUpdated())
		{
			DrawObjects(false);
			_needAllParticlesDraw = true;
		}
		else
		{
			DrawObjects(true);
			_needAllParticlesDraw = false;
		}

		GLFBOManager::GetInstance()->UnBindDefaultMSFBO();	
	}

	//Draw on default framebuffer
	glClearColor(0.3, 0.3, 0.35, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam2D::GetInstance()->SetProjection();

	_texture->SetBounds(0, 0, _sw, _sh);
	_texture->GetShader()->SetTextureID(GLFBOManager::GetInstance()->GetDefaultMSFBOTexID());
	_texture->GetShader()->Set2DCamera(true);

	bool depthTest = GLState::GLEnable(GL_DEPTH_TEST, false);
	bool cullFace = GLState::GLEnable(GL_CULL_FACE, false);
	_texture->Draw();
	GLState::GLEnable(GL_DEPTH_TEST, depthTest);
	GLState::GLEnable(GL_CULL_FACE, cullFace);
}

void ECoatPostProcessing::DrawObjects(bool drawAllParticles)
{
	_floor->Draw();

	_particleMgr->Draw(drawAllParticles);

	if(_drawAnodes)
	{
		std::vector<GLMeshRenderer*>* sources = _assetsBuilder->GetSources();
		for (int i = 0; i < sources->size(); i++)
		{
			sources->at(i)->Draw();
		}
	}

	GLMeshRenderer* fluid = _assetsBuilder->GetFluid();
	if (fluid && fluid->IsVisible())
	{
		bool culling = GLState::GLEnable(GL_CULL_FACE, true);
		GLenum cullFace = GLState::GLCullFace(GL_FRONT);
		fluid->Draw();
		GLState::GLCullFace(cullFace);
		GLState::GLEnable(GL_CULL_FACE, culling);
	}

	_colorBar->Draw();

	if (_showThickness)
	{
		bool depthTest = GLState::GLEnable(GL_DEPTH_TEST, false);
		bool cullFace = GLState::GLEnable(GL_CULL_FACE, false);

		std::string thicknessStr = StringUtils::doubleToStr(_selectedThickness, 8);
		GLFont::GetInstance()->Begin();
		GLFont::GetInstance()->DrawFromLeft(thicknessStr, _posForThickness.x, _posForThickness.y, 19);
		GLFont::GetInstance()->End();
		_showThickness = false;

		GLState::GLEnable(GL_DEPTH_TEST, depthTest);
		GLState::GLEnable(GL_CULL_FACE, cullFace);
	}
}

void ECoatPostProcessing::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();
	string comName = com->GetName();

	bool needRefresh = true;

	if (com == _timeLineFrame->nextFrame)
	{
		int selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		selectedFrameIndex++;

		if (selectedFrameIndex < _timeLineFrame->selectedFrame->Size())
		{
			_particleMgr->ApplyContour(selectedFrameIndex + 1);
			_timeLineFrame->selectedFrame->SetSelect(selectedFrameIndex);
			UpdateSolidPivot();
		}
	}
	else if (com == _timeLineFrame->previousFrame)
	{
		int selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		selectedFrameIndex--;

		if (selectedFrameIndex >= 0)
		{
			_particleMgr->ApplyContour(selectedFrameIndex + 1);
			_timeLineFrame->selectedFrame->SetSelect(selectedFrameIndex);
			UpdateSolidPivot();
		}
	}
	else if (com == _timeLineFrame->selectedFrame)
	{
		auto selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		_particleMgr->ApplyContour(selectedFrameIndex+1);
		UpdateSolidPivot();
	}

	/////////////// Visibility Settings /////////////////////////////

	else if (com == _timeLineFrame->visibilityBox->floor)
	{
		_floor->SetVisible(_timeLineFrame->visibilityBox->floor->IsSelected());
	}
	else if (com == _timeLineFrame->visibilityBox->fluid)
	{
		bool isSelected = _timeLineFrame->visibilityBox->fluid->IsSelected();
		_assetsBuilder->GetFluid()->SetVisible(isSelected);
	}
	else if (com == _timeLineFrame->visibilityBox->anodes)
	{
		_drawAnodes = _timeLineFrame->visibilityBox->anodes->IsSelected();
	}
	else if (com == _timeLineFrame->visibilityBox->solid)
	{
		bool isSelected = _timeLineFrame->visibilityBox->solid->IsSelected();
		_particleMgr->SetVisible(isSelected);
	}
	else if (com == _timeLineFrame->visibilityBox->colorBar)
	{
		bool isSelected = _timeLineFrame->visibilityBox->colorBar->IsSelected();
		_colorBar->SetVisible(isSelected);
	}

	/////////////// Camera Settings /////////////////////////////

	else if (com == _timeLineFrame->camBox->fontView)
	{
		Cam::GetInstance()->SetRightView();
	}
	else if (com == _timeLineFrame->camBox->backView)
	{
		Cam::GetInstance()->SetLeftView();
	}
	else if (com == _timeLineFrame->camBox->leftView)
	{
		Cam::GetInstance()->SetFrontView();
	}
	else if (com == _timeLineFrame->camBox->rightView)
	{
		Cam::GetInstance()->SetBackView();
	}
	else if (com == _timeLineFrame->camBox->topView)
	{
		Cam::GetInstance()->SetTopView();
	}
	else if (com == _timeLineFrame->camBox->bottomView)
	{
		Cam::GetInstance()->SetBottomView();
	}
	else if (com == _timeLineFrame->camBox->changeView)
	{
		Cam::GetInstance()->ChangeView();
	}
	else if (com == _timeLineFrame->camBox->resetPos)
	{
		Cam::GetInstance()->ResetToInitialPos();
		_timeLineFrame->camBox->pivotRadioBtn->SetSelect(0);
	}
	else if (com == _timeLineFrame->camBox->pivotRadioBtn)
	{
		Cam::GetInstance()->ResetToInitialPos();
		UpdateSolidPivot();
	}

	//////////////////  Solid Render Options /////////////////

	else if (com == _timeLineFrame->solidRenderBox->radioBtn)
	{
		int selectedIndex = _timeLineFrame->solidRenderBox->radioBtn->GetSelectedIndex();

		if (selectedIndex == 0)
		{
			_particleMgr->SetDrawAs(ECoatParticleDataMgr::DRAW_AS_CUBES);
		}
		else if (selectedIndex == 1)
		{
			_particleMgr->SetDrawAs(ECoatParticleDataMgr::DRAW_AS_CUBES_WITH_LIGHTING);
		}
		else if (selectedIndex == 2)
		{
			_particleMgr->SetDrawAs(ECoatParticleDataMgr::DRAW_AS_STL);
		}
	}
	else
	{
		needRefresh = false;
	}

	if(needRefresh)
		_needAllParticlesDraw = true;
}

void ECoatPostProcessing::UpdateSolidPivot()
{
	int selectedIndex = _timeLineFrame->camBox->pivotRadioBtn->GetSelectedIndex();

	if (selectedIndex == 0)
	{
		Cam::GetInstance()->SetPivot(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	else if (selectedIndex == 1)
	{
		Cam::GetInstance()->SetPivot(_particleMgr->GetBBoxCenterAfterTransform());
	}
}

ECoatPostProcessing::~ECoatPostProcessing()
{
	if (_particleMgr)
	{
		delete _particleMgr;
		_particleMgr = NULL;
	}

	if (_timeLineFrame)
	{
		delete _timeLineFrame;
		_timeLineFrame = nullptr;
	}

	if (_floor)
	{
		delete _floor;
		_floor = nullptr;
	}

	if (_colorBar)
	{
		delete _colorBar;
		_colorBar = nullptr;
	}

	if (_meshManager)
	{
		delete _meshManager;
		_meshManager = nullptr;
	}

	if (_carBody)
	{
		delete _carBody;
		_carBody = nullptr;
	}

	if (_assetsBuilder)
	{
		delete _assetsBuilder;
		_assetsBuilder = nullptr;
	}

	if (_texture)
	{
		delete _texture;
		_texture = nullptr;
	}
}


	//else if (Input::IsMouseClicked())
	//{
	//	int index = _meshManager->GetModelIndexByMousePos(Input::MX, Input::MY);
	//	printf("\nModel index from GLMeshManager = %d", index);
	//}
