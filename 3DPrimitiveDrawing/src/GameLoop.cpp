#include "GameLoop.h"

GameLoop::GameLoop(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_useFBO = false;

	GLSettings();

	_zNear = 1.0f;
	_zFar = 1000000.0f;
	_zNearPlaneW = 0.2f;

	_cam = Cam::GetInstance();
	_cam->Init(_sw, _sh, _zNear, _zFar, _zNearPlaneW);

	_cam2D = Cam2D::GetInstance();
	_cam2D->Init(_sw, _sh);

	_floor = new Floor();

	glm::vec3 v1(0.0f, 0.0f, 0.0f);
	glm::vec3 v2(0.0f, 5.0f, 0.0f);
	glm::vec3 v3(5.0f, 0.0f, 0.0f);

	_triangle = new Triangle(v1, v2, v3);

	_box = new Box(0, 0, 0, 2, 3, 4);
	_box->SetSize(3, 1, 6);
	_box->SetPos(-10, 0, -10);

	_cylinder = new Cylinder(0, 0, 0, 3, 2);
	_cylinder->SetRadius(1.5);
	_cylinder->SetHeight(2);
	_cylinder->SetPos(0, 0, -10);

	_cone = new Cone(0, 0, 0, 2, 3);
	_cone->SetRadius(1.5);
	_cone->SetHeight(2);
	_cone->SetPos(-5, 0, -10);

	_sphere = new Sphere(0, 0, 0, 2);
	_sphere->SetPos(5, 0, 0);
	_sphere->SetRadius(5);

	//_objLoader = new ObjLoader("data/teapot", false);
	//_binaryObjLoader = new BinaryObjLoader("data/carScaled");

	_fbo = new GLFBO(_sw*0.8, _sh*0.8);
	_texture = new GLTexture(_sw, _sh);

	_particleLoader = new ParticleLoader();
}

void GameLoop::GLSettings()
{
	//unsigned int framebuffer;
	//glGenFramebuffersEXT(1, &framebuffer);
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

	//glShadeModel(GL_SMOOTH);
	//glFrontFace	( GL_CCW		);
	//glDisable(GL_FOG);
	//glDisable(GL_LIGHTING);

	//glDisable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameLoop::Update(float deltaTime)
{
}

void GameLoop::Draw()
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	if (_useFBO)
	{
		_fbo->BindFBO();

		glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _fbo->GetW(), _fbo->GetH());

		SetCamAndDrawObjects();

		_fbo->UnBindFBO();
	}
	else
	{
		glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _sw, _sh);

		SetCamAndDrawObjects();
	}
	

	if (_useFBO)
	{
		glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _sw, _sh);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		_cam2D->SetProjection();

		glBindTexture(GL_TEXTURE_2D, _fbo->GetTextureID());
		_texture->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GameLoop::SetCamAndDrawObjects()
{
	_cam->SetPerspectiveProjection();
	_cam->SetViewMatrix();
	_cam->UpdateCamera();

	_floor->Draw();
	_particleLoader->Draw();

	//_objLoader->Draw();
	//_binaryObjLoader->Draw();

	//_triangle->Draw();
	//_box->Draw();
	//_cylinder->Draw();
	//_cone->Draw();
	//_sphere->Draw();
}

void GameLoop::SetScreenSize(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
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

	Cam::GetInstance()->DeleteInstance();
	ShadersManager::GetInstance()->DeleteInstance();
}
