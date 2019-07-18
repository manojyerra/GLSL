#include "ParticlesDemo.h"

ParticlesDemo::ParticlesDemo(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_fbo = NULL;
	_texture = NULL;

	_numLoaders = 1;
	_particleLoaderVec.clear();

	////

	_fbo = new GLFBO(_sw, _sh);
	_texture = new GLTexture("data/Sample.png", 0, 0, _sw, _sh);
	_texture->GetShader()->Set2DCamera(true);

	_floor = new Floor();

	for (int i = 0; i < _numLoaders; i++)
	{
		_particleLoaderVec.push_back(new ParticleLoader());
	}

	if (_particleLoaderVec.size() >= 2)	_particleLoaderVec[1]->SetPosition(2, 0, 0);
	if (_particleLoaderVec.size() >= 3)	_particleLoaderVec[2]->SetPosition(4, 0, 0);
	if (_particleLoaderVec.size() >= 4)	_particleLoaderVec[3]->SetPosition(-2, 0, 0);
	if (_particleLoaderVec.size() >= 5)	_particleLoaderVec[4]->SetPosition(-4, 0, 0);

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
		glClearColor(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _sw, _sh);

		_floor->Draw();

		for (int i = 0; i < _particleLoaderVec.size(); i++)
		{
			_particleLoaderVec[i]->DrawLowPolyParticles();
		}

		_drawAllParticles = true;
	}
	else
	{
		if (_drawAllParticles)
		{
			_fbo->BindFBO();

			glClearColor(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, _fbo->GetW(), _fbo->GetH());

			_floor->Draw();

			for (int i = 0; i < _particleLoaderVec.size(); i++)
			{
				_particleLoaderVec[i]->DrawAllParticles();
			}

			_fbo->UnBindFBO();
			_drawAllParticles = false;
		}

		glClearColor(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _sw, _sh);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		Cam2D::GetInstance()->SetProjection();

		_texture->SetBounds(0, 0, _sw, _sh);
		_texture->GetShader()->SetTextureID(_fbo->GetTextureID());
		_texture->GetShader()->Set2DCamera(true);

		_texture->Draw();
	}
}

void ParticlesDemo::SetScreenSize(int sw, int sh)
{
	char arr[1024];
	sprintf(arr, "\nOnSizeChange : sw = %d sh = %d", sw, sh);
	OutputDebugStringA(arr);

	_sw = sw;
	_sh = sh;

	if (_fbo)
	{
		delete _fbo;
		_fbo = NULL;
	}

	if (!_fbo)
	{
		_fbo = new GLFBO(_sw, _sh);
	}

	_drawAllParticles = true;
}

ParticlesDemo::~ParticlesDemo()
{
	for (int i = 0; i < _particleLoaderVec.size(); i++)
	{
		if (_particleLoaderVec[i])
		{
			delete _particleLoaderVec[i];
			_particleLoaderVec[i] = NULL;
		}
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