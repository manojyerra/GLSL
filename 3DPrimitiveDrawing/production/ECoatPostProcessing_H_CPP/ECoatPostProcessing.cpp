#include "ECoatPostProcessing.h"
#include "Cam.h"
#include "Cam2D.h"
#include "ParticleRenderer.h"
#include "ECoatResultReader.h"
#include "GLFBOManager.h"
#include "Input.h"
#include "ContourMap.h"
#include "ObjReader.h"
#include "STLReader.h"
#include "STLReaderWithThreads.h"
#include "GLState.h"

ECoatPostProcessing::ECoatPostProcessing(unsigned int sw, unsigned int sh, int argc, char** argv)
{
	_sw = sw;
	_sh = sh;

	float zNear = 0.15f;
	float zFar = 1000000.0f;
	float zNearPlaneW = 0.25f;

	Cam::GetInstance()->Init(_sw, _sh, zNear, zFar, zNearPlaneW);
	Cam2D::GetInstance()->Init(_sw, _sh);

	SetGLStates();
	SetScreenSize(_sw, _sh);

	//Begin: Default initialization 
	_floor = nullptr;
	_meshManager = nullptr;
	_carBody = nullptr;
	_assetsBuilder = nullptr;
	//End: Default initialization 


	_floor = new Floor();
	_meshManager = new GLMeshManager(_sw, _sh);

	_assetsBuilder = new ECoatAssetsBuilder(&ECoatAssetsReader("AdvancedRenderer_JLR_ECoating.json"), _meshManager);
	_resultReader = _assetsBuilder->GetResultReader();
	_colorBar = new ColorBar();

	_timeLineFrame = new TimeLineFrame(0, 0, 300, 700, _resultReader->GetFrameCount(), this);

	STLReaderWithThreads reader("AdvancedRendererInputFiles/checkedAuto_solidMesh_exported.stl");
	//STLReaderWithThreads reader("data/OpelGlandX.stl");

	//float* stlVertexArr = (float*)reader.GetVertexBuffer();
	//unsigned int stlVertexArrSize = reader.GetVertexBufferSize() / 4;
	//unsigned int stlVertexCount = reader.GetVertexBufferSize() / 12;

	//unsigned int stlColorBufLen = stlVertexCount * 3;
	//unsigned char* stlColorBuf = (unsigned char*)malloc(stlColorBufLen);
	//memset(stlColorBuf, '\0', stlColorBufLen);

	unsigned int parVertexBufSize;
	char* parVertexBuf = _resultReader->GetParticleBufferWorkpiece(1, &parVertexBufSize);

	unsigned int parColorBufLen = 0;
	unsigned char* parColorBuf = (unsigned char*)GetParticleColorBuf(80, &parColorBufLen);

	unsigned int parTriIDBufSize;
	char* parTriIDBuf = _resultReader->GetTriangleIDBufferWorkpiece(1, &parTriIDBufSize);

	//Begin : Generate particle normals data

	float* stlNormalsArr = (float*)reader.GetNormalBuffer();
	unsigned int stlNormalArrSize = reader.GetNormalBufferSize() / 4;

	unsigned int* triIDArr = (unsigned int*)parTriIDBuf;
	unsigned int numTriIDS = parTriIDBufSize / 4;

	long startTime = Platform::GetTimeInMillis();

	float* parNormalArr = (float*)malloc(parVertexBufSize);
	memset(parNormalArr, '\0', parVertexBufSize);

	for (int i = 0; i < numTriIDS; i++)
	{
		unsigned int triID = triIDArr[i];
		
		int stlNorIndex = triID * 9;
		int parNorIndex = i * 3;
		
		//parNormalArr[parNorIndex + 0] = stlNormalsArr[stlNorIndex + 0];
		//parNormalArr[parNorIndex + 1] = stlNormalsArr[stlNorIndex + 1];
		//parNormalArr[parNorIndex + 2] = stlNormalsArr[stlNorIndex + 2];

		memcpy(&parNormalArr[parNorIndex], &stlNormalsArr[stlNorIndex], 12);
	}

	printf("\nTimeTaken for copying normals: %ld", (Platform::GetTimeInMillis()-startTime));

	//End : Generate particle normals data


	////ContourMap colorMap(stlVertexArr, stlVertexArrSize, stlColorBuf, stlColorBufLen, (float*)parVertexBuf, parVertexBufSize / 4, parColorBuf, parColorBufLen);

	//BaseModelIO modelIO;
	//modelIO.SetVertexBuffer(reader.GetVertexBuffer(), reader.GetVertexBufferSize());
	//modelIO.SetNormalBuffer(reader.GetNormalBuffer(), reader.GetNormalBufferSize());
	//modelIO.SetColorBuffer((const char*)stlColorBuf, stlColorBufLen);

	//_carBody = new GLMeshRenderer(&modelIO, PBR_SHADER);

	_particleRenderer = new ParticleRenderer(parVertexBuf, parVertexBufSize, (char*)parNormalArr, parVertexBufSize);
	_particleRenderer->UpdateColorBuffer((char*)parColorBuf, parColorBufLen);

	free(parVertexBuf);

	_texture = new GLTexture(0.0f, 0.0f, _sw, _sh);
	_needAllParticlesDraw = true;

	//ApplyContour(80);
}

void ECoatPostProcessing::SetScreenSize(unsigned int sw, unsigned int sh)
{
	_sw = sw;
	_sh = sh;

	glViewport(0, 0, _sw, _sh);

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
	Cam2D::GetInstance()->SetScreenSize(_sw, _sh);
}

void ECoatPostProcessing::SetGLStates()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ECoatPostProcessing::OnModuleChange(unsigned int sw, unsigned int sh)
{
}

void ECoatPostProcessing::Update(float deltaTime)
{
}

void ECoatPostProcessing::Draw()
{
	Cam* cam = Cam::GetInstance();
	cam->SetPerspectiveProjection();
	cam->SetViewMatrix();
	cam->UpdateCamera();

	if (cam->IsCameraUpdated() || _needAllParticlesDraw)
	{
		GLFBOManager::GetInstance()->BindDefaultMSFBO();

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(cam->IsCameraUpdated())
		{
			DrawObjects(true);
			_needAllParticlesDraw = true;
		}
		else
		{
			DrawObjects(true);
			_needAllParticlesDraw = false;
		}

		GLFBOManager::GetInstance()->UnBindDefaultMSFBO();	
	}

	//Draw on default framebuffer
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam2D::GetInstance()->SetProjection();

	_texture->SetBounds(0, 0, _sw, _sh);
	_texture->GetShader()->SetTextureID(GLFBOManager::GetInstance()->GetDefaultMSFBOTexID());
	_texture->GetShader()->Set2DCamera(true);

	bool depthTest = GLState::GLEnable(GL_DEPTH_TEST, false);
	bool cullFace = GLState::GLEnable(GL_CULL_FACE, false);
	_texture->Draw();
	GLState::GLEnable(GL_DEPTH_TEST, depthTest);
	GLState::GLEnable(GL_CULL_FACE, cullFace);
}

void ECoatPostProcessing::DrawObjects(bool drawAllParticles)
{
	if (Input::IsMouseClicked())
	{
		int index = _meshManager->GetModelIndexByMousePos(Input::MX, Input::MY);
		printf("\nModel index from GLMeshManager = %d", index);
	}

	//_floor->Draw();

	//for (int i = 0; i < _meshManager->Size(); i++)
	//{
	//	_meshManager->Get(i)->Draw();
	//}

	if(drawAllParticles)
	{
		_particleRenderer->DrawAllParticles();
	}
	else
	{
		_particleRenderer->DrawFewParticles();
	}

	_colorBar->Draw();
}

void ECoatPostProcessing::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();

	if (com == _timeLineFrame->nextFrame)
	{
		auto selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		selectedFrameIndex++;

		ApplyContour(selectedFrameIndex + 1);

		_timeLineFrame->selectedFrame->SetSelect(selectedFrameIndex);
	}
	else if (com == _timeLineFrame->selectedFrame)
	{
		auto selectedFrameIndex = _timeLineFrame->selectedFrame->GetSelectedIndex();
		ApplyContour(selectedFrameIndex+1);
	}
}

ECoatPostProcessing::~ECoatPostProcessing()
{
	if (_particleRenderer)
	{
		delete _particleRenderer;
		_particleRenderer = NULL;
	}

	if (_timeLineFrame)
	{
		delete _timeLineFrame;
		_timeLineFrame = nullptr;
	}

	if (_floor)
	{
		delete _floor;
		_floor = nullptr;
	}

	if (_colorBar)
	{
		delete _colorBar;
		_colorBar = nullptr;
	}

	if (_meshManager)
	{
		delete _meshManager;
		_meshManager = nullptr;
	}

	if (_carBody)
	{
		delete _carBody;
		_carBody = nullptr;
	}

	if (_assetsBuilder)
	{
		delete _assetsBuilder;
		_assetsBuilder = nullptr;
	}
}



//
//glm::vec3 sCol(0.0f, 0.0f, 255.0f);
//glm::vec3 eCol(255.0f, 0.0f, 0.0f);
//
//float totDiffR = eCol.r - sCol.r;
//float totDiffG = eCol.g - sCol.g;
//float totDiffB = eCol.b - sCol.b;
//
//float factorR = totDiffR / totDiffThick;
//float factorG = totDiffG / totDiffThick;
//float factorB = totDiffB / totDiffThick;
//
//for (int i = 0; i < numThicknessVals; i++)
//{
//	int ii = i * 3;
//
//	colorBuf[ii + 0] = sCol.r + factorR * thickness;
//	colorBuf[ii + 1] = sCol.g + factorG * thickness;
//	colorBuf[ii + 2] = sCol.b + factorB * thickness;
//
//	float r = 0;
//	float g = 0;
//	float b = 0;
//
//	_colorBar->GetColor(thickness, &r, &g, &b);
//
//	colorBuf[ii + 0] = r * 255;
//	colorBuf[ii + 1] = g * 255;
//	colorBuf[ii + 2] = b * 255;
//}
//