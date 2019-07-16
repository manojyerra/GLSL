#include "GameLoop.h"
#include "GLMemory.h"
#include "ObjReader.h"

GameLoop::GameLoop(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_zNear = 1.0f;
	_zFar = 1000000.0f;
	_zNearPlaneW = 0.2f;

	_useFBO = false;

	GLSettings();

	_floor = NULL;
	_box = NULL;
	_cylinder = NULL;
	_cone = NULL;
	_sphere = NULL;
	_fbo = NULL;
	_texture = NULL;

	Cam::GetInstance()->Init(_sw, _sh, _zNear, _zFar, _zNearPlaneW);
	Cam2D::GetInstance()->Init(_sw, _sh);

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

	_fbo = new GLFBO(_sw, _sh);
	_texture = new GLTexture(_sw, _sh);

	_particleLoader = new ParticleLoader();
	//_particleLoader2 = new ParticleLoader();
	//_particleLoader3 = new ParticleLoader();
	//_particleLoader4 = new ParticleLoader();
	//_particleLoader5 = new ParticleLoader();

	//_particleLoader2->SetPosition(0, 2, 0);
	//_particleLoader3->SetPosition(0, 4, 0);
	//_particleLoader4->SetPosition(0, -2, 0);
	//_particleLoader5->SetPosition(0, -4, 0);

	_drawAllParticles = true;

	//_meshRenderer->SetShader(GLMeshRenderer::BASIC_SHADER);
	//_meshRenderer->SetShader(GLMeshRenderer::PHONG_PER_VERTEX_SHADER);
	//_meshRenderer->SetShader(GLMeshRenderer::PHONG_PER_PIXEL_SHADER);

	_meshRenderer = new GLMeshRenderer(&ObjReader("data/alien"));
	_meshRenderer->SetShader(GLMeshRenderer::PBR_SHADER);

	_meshRenderer1 = new GLMeshRenderer(&BinaryObjReader("data/alien"));
	_meshRenderer1->SetShader(GLMeshRenderer::PHONG_PER_VERTEX_SHADER);
	_meshRenderer1->SetPos(8.0,0.0,0.0);
}

void GameLoop::GLSettings()
{
	//glShadeModel(GL_SMOOTH);
	//glDisable(GL_FOG);

	//glDisable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameLoop::Update(float deltaTime)
{
}

void GameLoop::ParticleSpecificDraw()
{
	Cam::GetInstance()->SetPerspectiveProjection();
	Cam::GetInstance()->SetViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	if (Cam::GetInstance()->IsCameraUpdated())
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _fbo->GetW(), _fbo->GetH());

		_floor->Draw();
		_particleLoader->DrawLowPolyParticles();
		//_particleLoader2->DrawLowPolyParticles();
		//_particleLoader3->DrawLowPolyParticles();
		//_particleLoader4->DrawLowPolyParticles();
		//_particleLoader5->DrawLowPolyParticles();

		_drawAllParticles = true;
	}
	else
	{
		if (_drawAllParticles)
		{
			_fbo->BindFBO();

			glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, _fbo->GetW(), _fbo->GetH());

			_floor->Draw();
			_particleLoader->DrawAllParticles();
			//_particleLoader2->DrawAllParticles();
			//_particleLoader3->DrawAllParticles();
			//_particleLoader4->DrawAllParticles();
			//_particleLoader5->DrawAllParticles();

			_fbo->UnBindFBO();
			_drawAllParticles = false;
		}

		glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _sw, _sh);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		Cam2D::GetInstance()->SetProjection();

		glBindTexture(GL_TEXTURE_2D, _fbo->GetTextureID());
		_texture->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GameLoop::Draw()
{
	//ParticleSpecificDraw();
	//return;

	glEnable(GL_DEPTH_TEST);

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

		Cam2D::GetInstance()->SetProjection();

		glBindTexture(GL_TEXTURE_2D, _fbo->GetTextureID());
		_texture->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GameLoop::SetCamAndDrawObjects()
{
	Cam::GetInstance()->SetPerspectiveProjection();
	Cam::GetInstance()->SetViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	_floor->Draw();
	//_objLoader->Draw();
	//_binaryObjLoader->Draw();
	//_box->Draw();
	//_cone->Draw();
	//_sphere->Draw();
	//_cylinder->Draw();
	_meshRenderer->Draw();
	_meshRenderer1->Draw();
}

void GameLoop::SetScreenSize(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
}

GameLoop::~GameLoop()
{
	if (_particleLoader)
	{
		delete _particleLoader;
		_particleLoader = NULL;
	}

	if (_texture)
	{
		delete _texture;
		_texture = NULL;
	}

	if (_fbo)
	{
		delete _fbo;
		_fbo = NULL;
	}
	
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

	if (_meshRenderer)
	{
		delete _meshRenderer;
		_meshRenderer = NULL;
	}

	if (_meshRenderer1)
	{
		delete _meshRenderer1;
		_meshRenderer1 = NULL;
	}

	Cam::GetInstance()->DeleteInstance();
	Cam2D::GetInstance()->DeleteInstance();
	ShadersManager::GetInstance()->DeleteInstance();

	GLMemory::printMemoryLeaks();
}
