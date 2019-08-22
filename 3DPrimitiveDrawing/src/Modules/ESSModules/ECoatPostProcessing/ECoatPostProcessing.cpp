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

ECoatPostProcessing::ECoatPostProcessing(unsigned int sw, unsigned int sh, int argc, char** argv)
{
	_sw = sw;
	_sh = sh;

	float zNear = 0.15f;
	float zFar = 1000000.0f;
	float zNearPlaneW = 0.25f;

	Cam::GetInstance()->Init(_sw, _sh, zNear, zFar, zNearPlaneW);
	Cam2D::GetInstance()->Init(_sw, _sh);

	SetGLStates();
	SetScreenSize(_sw, _sh);

	//Begin: Default initialization 
	_floor = nullptr;
	_meshManager = nullptr;
	_carBody = nullptr;
	_assetsBuilder = nullptr;
	_particleMgr = nullptr;
	//End: Default initialization 


	_floor = new Floor();
	_meshManager = new GLMeshManager(_sw, _sh);

	_assetsBuilder = new ECoatAssetsBuilder(&ECoatAssetsReader("AdvancedRenderer_JLR_ECoating.json"), _meshManager);
	_colorBar = new ColorBar();
	_timeLineFrame = new TimeLineFrame(0, 0, 300, 700, _assetsBuilder->GetResultReader()->GetTotalFrameCount(), this);

	_particleMgr = new ECoatParticleDataMgr(_assetsBuilder, _colorBar);

	_texture = new GLTexture(0.0f, 0.0f, _sw, _sh);
	_needAllParticlesDraw = true;
}

void ECoatPostProcessing::SetScreenSize(unsigned int sw, unsigned int sh)
{
	_sw = sw;
	_sh = sh;

	glViewport(0, 0, _sw, _sh);

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
	Cam2D::GetInstance()->SetScreenSize(_sw, _sh);
}

void ECoatPostProcessing::SetGLStates()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
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

	if (cam->IsCameraUpdated() || _needAllParticlesDraw)
	{
		GLFBOManager::GetInstance()->BindDefaultMSFBO();

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
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
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
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
	if (Input::IsMouseClicked())
	{
		int index = _meshManager->GetModelIndexByMousePos(Input::MX, Input::MY);
		printf("\nModel index from GLMeshManager = %d", index);
	}

	//_floor->Draw();

	//for (int i = 0; i < _meshManager->Size(); i++)
	//{
	//	_meshManager->Get(i)->Draw();
	//}

	_particleMgr->Draw(drawAllParticles);

	//_colorBar->Draw();
}

void ECoatPostProcessing::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();

	if (com == _timeLineFrame->nextFrame)
	{
		int selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		selectedFrameIndex++;

		_particleMgr->ApplyContour(selectedFrameIndex + 1);
		_timeLineFrame->selectedFrame->SetSelect(selectedFrameIndex);
		_needAllParticlesDraw = true;
	}
	else if (com == _timeLineFrame->selectedFrame)
	{
		auto selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		_particleMgr->ApplyContour(selectedFrameIndex+1);
		_needAllParticlesDraw = true;
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
