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

	quadVAO = 0;
	quadVBO = 0;

	_zNear = 0.15f;
	_zFar = 1000000.0f;
	_zNearPlaneW = 0.2f;

	glEnable(GL_DEPTH_TEST);

	shaderGeometryPass = new ShaderProgram("shaders/ssao/ssao_geometry.vs", "shaders/ssao/ssao_geometry.fs");
	shaderSSAO = new ShaderProgram("shaders/ssao/ssao.vs", "shaders/ssao/ssao.fs");

	nanosuit = new GLMeshRenderer(&ObjReader("data/demo/nanosuite"), GLMeshRenderer::PHONG_PER_PIXEL_SHADER);
	ssao = new GLSSAO(sw, sh);

	glGenVertexArrays(1, &vao);
	glDisable(GL_CULL_FACE);

	float _zNear = 0.15f;
	float _zFar = 1000000.0f;
	float _zNearPlaneW = 0.2f;

	Cam::GetInstance()->Init(sw, sh, _zNear, _zFar, _zNearPlaneW);
}

void GameLoop::Update(float deltaTime)
{
}

void GameLoop::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. geometry pass: render scene's geometry/color data into gbuffer
	// -----------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, ssao->GetGBufferFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam* cam = Cam::GetInstance();
	GLMat modelMat;

	cam->SetPerspectiveProjection();
	cam->SetViewMatrix();
	cam->UpdateCamera();

	shaderGeometryPass->Begin();
	shaderGeometryPass->SetUniformMatrix4fv("projection", cam->GetProjMat());
	shaderGeometryPass->SetUniformMatrix4fv("view", cam->GetViewMat());
	shaderGeometryPass->SetUniformMatrix4fv("model", modelMat.m);

	glBindVertexArray(vao);

	if (nanosuit->_bufferBuilder->GetNormalBufferID())
	{
		GLuint normalLoc = glGetAttribLocation(shaderGeometryPass->ProgramID(), "aNormal");
		glEnableVertexAttribArray(normalLoc);
		glBindBuffer(GL_ARRAY_BUFFER, nanosuit->_bufferBuilder->GetNormalBufferID());
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
	}

	GLuint vertexLoc = glGetAttribLocation(shaderGeometryPass->ProgramID(), "aPos");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, nanosuit->_bufferBuilder->GetVertexBufferID());
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GLMeshRenderer::triangles, 0, (GLsizei)(nanosuit->_bufferBuilder->GetVertexBufferSize() / 12));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. generate SSAO texture
	glClear(GL_COLOR_BUFFER_BIT);

	shaderSSAO->Begin();
	shaderSSAO->SetUniform1i("gPosition", 0);
	shaderSSAO->SetUniform1i("gNormal", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssao->GetGPositionTexID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ssao->GetGNormalTexID());

	renderQuad();
}

void GameLoop::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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