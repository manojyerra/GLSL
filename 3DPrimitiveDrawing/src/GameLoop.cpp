#include "GameLoop.h"
#include "GLMemory.h"
#include "ObjReader.h"
#include "BinaryObjReader.h"
#include "ShadersManager.h"
#include "Input.h"
#include "SUI.h"

GameLoop::GameLoop(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_zNear = 1.0f;
	_zFar = 1000000.0f;
	_zNearPlaneW = 0.2f;

	GLSettings();
	SUISetup(_sw, _sh);

	_floor = NULL;
	_box = NULL;
	_cylinder = NULL;
	_cone = NULL;
	_sphere = NULL;
	_triangle = NULL;

	Cam::GetInstance()->Init(_sw, _sh, _zNear, _zFar, _zNearPlaneW);
	Cam2D::GetInstance()->Init(_sw, _sh);

	_particleDemo = new ParticlesDemo(_sw, _sh);
	_rendererDemo = new RenderDemo(_sw, _sh);

	_windowFrame = new WholeWindowFrame(_sw - 300, 0.0f, 300, 130.0f, this);
}

void GameLoop::GLSettings()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameLoop::Update(float deltaTime)
{
	
}

void GameLoop::actionPerformed(SUIActionEvent e) 
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();

	if (com == _windowFrame->demoType) 
	{
		int index = _windowFrame->demoType->GetSelectedIndex();
		_rendererDemo->SetVisibleFrames(index == 0);
	}
	else if (com == _windowFrame->isSSAO) 
	{
		printf("SSAO enabled : %d", _windowFrame->isSSAO->IsSelected());
	}
}

void GameLoop::Draw()
{
	bool consumed = SUIInput::Update((float)Input::MX, (float)Input::MY, Input::LEFT_BUTTON_DOWN, 1.0f / 30.0f);;
	Input::SetEnable(!consumed);

	if (_windowFrame->GetDemoIndex() == 0)
	{
		_rendererDemo->Draw();
	}
	else if (_windowFrame->GetDemoIndex() == 1)
	{
		_particleDemo->Draw();
	}

	SUIDraw();
}

void GameLoop::SetScreenSize(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
	Cam2D::GetInstance()->SetScreenSize(_sw, _sh);

	SUISetWindowSize(_sw, _sh);

	_rendererDemo->SetScreenSize(_sw, _sh);
	_particleDemo->SetScreenSize(_sw, _sh);
	_windowFrame->SetPos(_sw - _windowFrame->GetWidth(), 0);
}

GameLoop::~GameLoop()
{
	if (_sphere != NULL)
	{
		delete _sphere;
		_sphere = NULL;
	}

	if (_cone != NULL)
	{
		delete _cone;
		_cone = NULL;
	}

	if (_cylinder != NULL)
	{
		delete _cylinder;
		_cylinder = NULL;
	}

	if (_box != NULL)
	{
		delete _box;
		_box = NULL;
	}

	if (_triangle != NULL)
	{
		delete _triangle;
		_triangle = NULL;
	}

	if (_floor != NULL)
	{
		delete _floor;
		_floor = NULL;
	}

	if (_particleDemo)
	{
		delete _particleDemo;
		_particleDemo = NULL;
	}

	if (_windowFrame) {
		delete _windowFrame;
		_windowFrame = NULL;
	}

	Cam::GetInstance()->DeleteInstance();
	Cam2D::GetInstance()->DeleteInstance();
	ShadersManager::GetInstance()->DeleteInstance();
	GLMemory::printMemoryLeaks();

	SUIQuit();
}


/*

	glEnable(GL_DEPTH_TEST);

	//if (_useFBO)
	//{
	//	_fbo->BindFBO();

	//	glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glViewport(0, 0, _fbo->GetW(), _fbo->GetH());

	//	SetCamAndDrawObjects();

	//	_fbo->UnBindFBO();
	//}
	//else
	//{
		glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _sw, _sh);

		SetCamAndDrawObjects();
	//}

	//if (_useFBO)
	//{
	//	glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glViewport(0, 0, _sw, _sh);
	//	glDisable(GL_DEPTH_TEST);
	//	glDisable(GL_CULL_FACE);

	//	Cam2D::GetInstance()->SetProjection();

	//	glBindTexture(GL_TEXTURE_2D, _fbo->GetTextureID());
	//	_texture->Draw();
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}

*/


//glm::vec3 v1(0.0f, 0.0f, 0.0f);
//glm::vec3 v2(0.0f, 5.0f, 0.0f);
//glm::vec3 v3(5.0f, 0.0f, 0.0f);

//_triangle = new Triangle(v1, v2, v3);

//_box = new Box(0, 0, 0, 2, 3, 4);
//_box->SetSize(3, 1, 6);
//_box->SetPos(-10, 0, -10);

//_cylinder = new Cylinder(0, 0, 0, 3, 2);
//_cylinder->SetRadius(1.5);
//_cylinder->SetHeight(2);
//_cylinder->SetPos(0, 0, -10);

//_cone = new Cone(0, 0, 0, 2, 3);
//_cone->SetRadius(1.5);
//_cone->SetHeight(2);
//_cone->SetPos(-5, 0, -10);

//_sphere = new Sphere(0, 0, 0, 2);
//_sphere->SetPos(5, 0, 0);
//_sphere->SetRadius(5);

