#include "RenderDemo.h"
#include "Cam.h"
#include "ObjReader.h"

RenderDemo::RenderDemo(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_floor = new Floor();
	_model1 = new GLMeshRenderer(&ObjReader("data/alien"), GLMeshRenderer::PBR_SHADER);
}

void RenderDemo::Draw()
{
	glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _sw, _sh);
	glEnable(GL_DEPTH_TEST);

	Cam::GetInstance()->SetPerspectiveProjection();
	Cam::GetInstance()->SetViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	_floor->Draw();
	_model1->Draw();
}

RenderDemo::~RenderDemo()
{
	if (_floor)
	{
		delete _floor;
		_floor = NULL;
	}

	if (_model1)
	{
		delete _model1;
		_floor = NULL;
	}
}
