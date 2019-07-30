#include "GameLoop.h"
#include "GLMemory.h"
#include "ShadersManager.h"
#include "Input.h"
#include "Cam.h"
#include "Cam2D.h"
#include "StringUtils.h"
//#include "ObjReader.h"
//#include "GLState.h"

GameLoop::GameLoop(float sw, float sh)
{
	//char s1[] = "87.549e2", s2[] = "-982.47e-3", s3[] = "-54e", s4[] = "-.64e4", s5[] = "+.87e+ 6";

	//float f1 = atof(s1);
	//float f2 = atof(s2);
	//float f3 = atof(s3);
	//float f4 = atof(s4);
	//float f5 = atof(s5);

	//float _f1 = StringUtils::atof_test(s1);
	//float _f2 = StringUtils::atof_test(s2);
	//float _f3 = StringUtils::atof_test(s3);
	//float _f4 = StringUtils::atof_test(s4);
	//float _f5 = StringUtils::atof_test(s5);


	_sw = sw;
	_sh = sh;

	_zNear = 0.15f;
	_zFar = 1000000.0f;
	_zNearPlaneW = 0.2f;

	Cam::GetInstance()->Init(sw, sh, _zNear, _zFar, _zNearPlaneW);
	Cam2D::GetInstance()->Init(sw, sh);

	_demo = new Demo(sw, sh);
}

void GameLoop::Update(float deltaTime)
{
}

void GameLoop::Draw()
{
	_demo->Draw();
}

void GameLoop::SetScreenSize(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
	Cam2D::GetInstance()->SetScreenSize(_sw, _sh);

	_demo->SetScreenSize(_sw, _sh);
}

GameLoop::~GameLoop()
{
	if(_demo)
	{
		delete _demo;
		_demo = nullptr;
	}

	Cam::GetInstance()->DeleteInstance();
	Cam2D::GetInstance()->DeleteInstance();
	ShadersManager::GetInstance()->DeleteInstance();
	GLMemory::printMemoryLeaks();
}


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