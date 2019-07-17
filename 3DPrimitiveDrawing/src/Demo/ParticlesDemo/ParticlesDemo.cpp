#include "ParticlesDemo.h"

ParticlesDemo::ParticlesDemo(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_fbo = NULL;
	_texture = NULL;

	_particleLoader1 = NULL;
	_particleLoader2 = NULL;
	_particleLoader3 = NULL;
	_particleLoader4 = NULL;
	_particleLoader5 = NULL;

	////

	_fbo = new GLFBO(sw, sh);
	_texture = new GLTexture("data/Sample.png", 0,0,_sw,_sh);
	_texture->GetShader()->Set2DCamera(true);

	_floor = new Floor();

	_particleLoader1 = new ParticleLoader();
	_particleLoader2 = new ParticleLoader();
	_particleLoader3 = new ParticleLoader();
	_particleLoader4 = new ParticleLoader();
	_particleLoader5 = new ParticleLoader();

	_particleLoader2->SetPosition(2, 0, 0);
	_particleLoader3->SetPosition(4, 0, 0);
	_particleLoader4->SetPosition(-2, 0, 0);
	_particleLoader5->SetPosition(-4, 0, 0);

	_drawAllParticles = true;
}


void ParticlesDemo::Draw()
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

		_particleLoader1->DrawLowPolyParticles();
		_particleLoader2->DrawLowPolyParticles();
		_particleLoader3->DrawLowPolyParticles();
		_particleLoader4->DrawLowPolyParticles();
		_particleLoader5->DrawLowPolyParticles();

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

			_particleLoader1->DrawAllParticles();
			_particleLoader2->DrawAllParticles();
			_particleLoader3->DrawAllParticles();
			_particleLoader4->DrawAllParticles();
			_particleLoader5->DrawAllParticles();

			_fbo->UnBindFBO();
			_drawAllParticles = false;
		}

		glClearColor(57.0f / 255.0f, 57.0f / 255.0f, 57.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _sw, _sh);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		Cam2D::GetInstance()->SetProjection();

		_texture->GetShader()->SetTextureID(_fbo->GetTextureID());
		_texture->GetShader()->Set2DCamera(true);

		_texture->Draw();		
	}
}

ParticlesDemo::~ParticlesDemo()
{
	if (_particleLoader1)
	{
		delete _particleLoader1;
		_particleLoader1 = NULL;
	}

	if (_particleLoader2)
	{
		delete _particleLoader2;
		_particleLoader2 = NULL;
	}
	
	if (_particleLoader3)
	{
		delete _particleLoader3;
		_particleLoader3 = NULL;
	}
	
	if (_particleLoader4)
	{
		delete _particleLoader4;
		_particleLoader4 = NULL;
	}
	
	if (_particleLoader5)
	{
		delete _particleLoader5;
		_particleLoader5 = NULL;
	}

	if (_fbo)
	{
		delete _fbo;
		_fbo = NULL;
	}

	if (_texture)
	{
		delete _texture;
		_texture = NULL;
	}

	if (_floor)
	{
		delete _floor;
		_floor = NULL;
	}
}
