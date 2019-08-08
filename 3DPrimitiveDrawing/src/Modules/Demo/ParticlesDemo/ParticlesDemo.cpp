#ifdef _ENABLE_DEMO

#include "ParticlesDemo.h"
#include "Cam.h"
#include "Cam2D.h"
#include "GLState.h"

ParticlesDemo::ParticlesDemo(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	_fbo = NULL;
	_texture = NULL;

	_numLoaders = 1;
	_particleRendererVec.clear();

	_fbo = new GLFBO((int)_sw, (int)_sh);
	_texture = new GLTexture(0.0f, 0.0f, _sw, _sh);
	_texture->GetShader()->Set2DCamera(true);

	_floor = new Floor();

	for (int i = 0; i < _numLoaders; i++)
	{
		_particleRendererVec.push_back(new ParticleRenderer(""));
	}

	if (_particleRendererVec.size() >= 2)	_particleRendererVec[1]->SetPosition(2, 0, 0);
	if (_particleRendererVec.size() >= 3)	_particleRendererVec[2]->SetPosition(-2, 0, 0);
	if (_particleRendererVec.size() >= 4)	_particleRendererVec[3]->SetPosition(4, 0, 0);
	if (_particleRendererVec.size() >= 5)	_particleRendererVec[4]->SetPosition(-4, 0, 0);

	_drawAllParticles = true;

	float zNear = 0.15f;
	float zFar = 1000000.0f;
	float zNearPlaneW = 0.25f;

	Cam::GetInstance()->Init(sw, sh, zNear, zFar, zNearPlaneW);
	Cam2D::GetInstance()->Init(sw, sh);
}

void ParticlesDemo::Draw()
{
	//TODO: Find what is the reason for disabling the cull face while drawing particles.
	bool cullFaceForDraw = GLState::GLEnable(GL_CULL_FACE, false);

	float clearValue = 110.0f / 255.0f;

	Cam::GetInstance()->SetPerspectiveProjection();
	Cam::GetInstance()->SetViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	if (Cam::GetInstance()->IsCameraUpdated())
	{
		glClearColor(clearValue, clearValue, clearValue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_floor->Draw();

		for (int i = 0; i < _particleRendererVec.size(); i++)
		{
			_particleRendererVec[i]->DrawFewParticles();
		}

		_drawAllParticles = true;
	}
	else
	{
		if (_drawAllParticles)
		{
			_fbo->BindFBO();

			glClearColor(clearValue, clearValue, clearValue, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			_floor->Draw();

			for (int i = 0; i < _particleRendererVec.size(); i++)
			{
				_particleRendererVec[i]->DrawAllParticles();
			}

			_fbo->UnBindFBO();
			_drawAllParticles = false;
		}

		glClearColor(clearValue, clearValue, clearValue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Cam2D::GetInstance()->SetProjection();

		_texture->SetBounds(0, 0, _sw, _sh);
		_texture->GetShader()->SetTextureID(_fbo->GetTextureID());
		_texture->GetShader()->Set2DCamera(true);

		bool depthTest = GLState::GLEnable(GL_DEPTH_TEST, false);
		bool cullFace = GLState::GLEnable(GL_CULL_FACE, false);
		_texture->Draw();
		GLState::GLEnable(GL_DEPTH_TEST, depthTest);
		GLState::GLEnable(GL_CULL_FACE, cullFace);
	}

	GLState::GLEnable(GL_CULL_FACE, cullFaceForDraw);
}

void ParticlesDemo::SetScreenSize(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	if (_fbo)
	{
		delete _fbo;
		_fbo = NULL;
	}

	if (!_fbo)
	{
		_fbo = new GLFBO((int)_sw, (int)_sh);
	}

	_drawAllParticles = true;

	glViewport(0, 0, (GLuint)_sw, (GLuint)_sh);
}

void ParticlesDemo::SetFloorVisible(bool visible)
{
	_floor->SetVisible(visible);
	_drawAllParticles = true;
}

Floor* ParticlesDemo::GetFloor()
{
	return _floor;
}

void ParticlesDemo::Reset()
{
	_drawAllParticles = true;
	_floor->SetVisible(true);
}

ParticlesDemo::~ParticlesDemo()
{
	for (int i = 0; i < _particleRendererVec.size(); i++)
	{
		if (_particleRendererVec[i])
		{
			delete _particleRendererVec[i];
			_particleRendererVec[i] = NULL;
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

#endif