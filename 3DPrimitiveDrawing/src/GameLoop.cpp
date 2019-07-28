#include "GameLoop.h"
#include "GLMemory.h"
#include "ShadersManager.h"
#include "Input.h"
#include "Cam.h"
#include "Cam2D.h"
#include "ObjReader.h"

GameLoop::GameLoop(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	_zNear = 0.15f;
	_zFar = 1000000.0f;
	_zNearPlaneW = 0.2f;

	Cam::GetInstance()->Init(sw, sh, _zNear, _zFar, _zNearPlaneW);
	Cam2D::GetInstance()->Init(sw, sh);

	nanosuit = new GLMeshRenderer(&ObjReader("data/demo/Trike"), GLMeshRenderer::SSAO_GEOMETRY_PASS_SHADER);
	ssao = new GLSSAO(sw, sh);

	_texture = new GLTexture("data/demo/Sample.png", 0, 0, sw, sh);
	_texture->GetShader()->Set2DCamera(true);
}

void GameLoop::Update(float deltaTime)
{
}

void GameLoop::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, _sw, _sh);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam* cam = Cam::GetInstance();
	cam->SetPerspectiveProjection();
	cam->SetViewMatrix();
	cam->UpdateCamera();

	//Geometry pass
	glBindFramebuffer(GL_FRAMEBUFFER, ssao->GetGBufferFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	nanosuit->SetShader(GLMeshRenderer::SSAO_GEOMETRY_PASS_SHADER);
	nanosuit->Draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Calculating SSAO
	glBindFramebuffer(GL_FRAMEBUFFER, ssao->GetSSAOFBO());
	glClear(GL_COLOR_BUFFER_BIT);
	ssao->DrawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Blur SSAO
	glBindFramebuffer(GL_FRAMEBUFFER, ssao->GetSSAOBlurFBO());
	glClear(GL_COLOR_BUFFER_BIT);
	ssao->DrawBlurQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	nanosuit->SetShader(GLMeshRenderer::PBR_SHADER);
	nanosuit->Draw();

	glEnable(GL_BLEND);

	static bool on = false;
	
	if (on)
	{
		glDisable(GL_DEPTH_TEST);
		Cam2D::GetInstance()->SetProjection();
		_texture->GetShader()->SetTextureID(ssao->GetSSAOBlurColorAttachmentID());
		_texture->Draw();
	}

	if (Input::IsMouseClicked())
	{
		on = !on;

		if(on)
			printf("\nocclustion is on");
		else
			printf("\nocclustion is off");
	}
}

void GameLoop::SetScreenSize(float sw, float sh)
{
	_sw = sw;
	_sh = sh;

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
	Cam2D::GetInstance()->SetScreenSize(_sw, _sh);
}

GameLoop::~GameLoop()
{
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