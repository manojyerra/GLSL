#ifdef _ENABLE_DEMO

#include "Input.h"
#include "Demo.h"
#include "GLMemory.h"
#include "Cam.h"
#include "SUI.h"
#include "GLState.h"

Demo::Demo(float sw, float sh)
{
	_floor = nullptr;

	_particleDemo = nullptr;
	_rendererDemo = nullptr;

	SUISetup(sw, sh);

	_rendererDemo = new RenderDemo(sw, sh);
	_particleDemo = new ParticlesDemo(sw, sh);

	_windowFrame = new WholeWindowFrame(sw - 300.0f, 0.0f, 300.0f, 150.0f, this);
	_floorFrame = new FloorVisibilityFrame(sw - 300.0f, 555.0f, 300.0f, 100.0f, this);
}

void Demo::Update(float deltaTime)
{
}

void Demo::actionPerformed(SUIActionEvent e) 
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();

	if (com == _windowFrame->demoType) 
	{
		int index = _windowFrame->demoType->GetSelectedIndex();
		
		if(_rendererDemo)
			_rendererDemo->SetVisibleFrames(index == 0);

		_floorFrame->SetVisible(index == 0);

		if(index == 0)
		{
			Cam::GetInstance()->SetTrans(glm::vec3(0, 0, -44.0f));
			Cam::GetInstance()->SetRot(glm::vec3(35.0f, -20.0f, 0));
			Cam::GetInstance()->UpdateCamera();
		}
		else
		{
			Cam::GetInstance()->SetTrans(glm::vec3(-0.095, -0.78, -4.94f));
			Cam::GetInstance()->SetRot(glm::vec3(31.0f, -35.0f, 0));
			Cam::GetInstance()->UpdateCamera();

			if(_particleDemo)
				_particleDemo->Reset();
			
		}
	}
	else if (com == _windowFrame->isSSAO) 
	{
		if (_rendererDemo)
			_rendererDemo->SetEnableSSAO(_windowFrame->isSSAO->IsSelected());
	}
	else if (com == _floorFrame->hideFloor)
	{
		bool hideFloor = _floorFrame->hideFloor->IsSelected();

		if (_windowFrame->GetDemoIndex() == 0)
		{
			if (_rendererDemo)
				_rendererDemo->SetFloorVisible(!hideFloor);
		}
		else if (_windowFrame->GetDemoIndex() == 1)
		{
			if (_particleDemo)
				_particleDemo->SetFloorVisible(!hideFloor);
		}
	}
	else if(com == _floorFrame->showOnlyGridLines)
	{
		bool showOnlyGridLines = _floorFrame->showOnlyGridLines->IsSelected();

		if (_windowFrame->GetDemoIndex() == 0)
		{
			if (_rendererDemo)
				_rendererDemo->GetFloor()->ShowOnlyGridLines(showOnlyGridLines);
		}
		else if (_windowFrame->GetDemoIndex() == 1)
		{
			if (_particleDemo)
				_particleDemo->GetFloor()->ShowOnlyGridLines(showOnlyGridLines);
		}
	}
}

void Demo::Draw()
{
	bool consumed = SUIInput::Update((float)Input::MX, (float)Input::MY, Input::LEFT_BUTTON_DOWN, 1.0f / 30.0f);
	Input::SetEnable(!consumed);

	if (_windowFrame->GetDemoIndex() == 0)
	{
		if(_rendererDemo)
			_rendererDemo->Draw();		
	}
	else if (_windowFrame->GetDemoIndex() == 1)
	{
		if(_particleDemo)
			_particleDemo->Draw();
	}

	bool multiSample = GLState::GLEnable(GL_MULTISAMPLE, false);
	SUIDraw();
	GLState::GLEnable(GL_MULTISAMPLE, multiSample);
}

void Demo::SetScreenSize(float sw, float sh)
{
	SUISetWindowSize(sw, sh);

	if(_rendererDemo)
		_rendererDemo->SetScreenSize(sw, sh);

	if(_particleDemo)
		_particleDemo->SetScreenSize(sw, sh);

	_windowFrame->SetPos(sw - _windowFrame->GetWidth(), 0);
	_floorFrame->SetPos(sw - _floorFrame->GetWidth(), 555);
}

Demo::~Demo()
{
	if (_floor != NULL)
	{
		delete _floor;
		_floor = NULL;
	}

	if (_rendererDemo)
	{
		delete _rendererDemo;
		_rendererDemo = NULL;
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

	if(_floorFrame)
	{
		delete _floorFrame;
		_floorFrame = NULL;
	}

	SUIQuit();
}

#endif