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

ECoatPost::ECoatPost(unsigned int sw, unsigned int sh, int argc, char** argv)
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
	_colorBar = new ECoatColorBar();

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

void ECoatPost::SetScreenSize(unsigned int sw, unsigned int sh)
{
	_sw = sw;
	_sh = sh;

	glViewport(0, 0, _sw, _sh);

	Cam::GetInstance()->SetScreenSize(_sw, _sh);
	Cam2D::GetInstance()->SetScreenSize(_sw, _sh);
}

void ECoatPost::SetGLStates()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ECoatPost::OnModuleChange(unsigned int sw, unsigned int sh)
{
}

void ECoatPost::Update(float deltaTime) 
{
}

void ECoatPost::Draw()
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

void ECoatPost::DrawObjects(bool drawAllParticles)
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

void ECoatPost::actionPerformed(SUIActionEvent e)
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

void ECoatPost::ApplyContour(int frameNum)
{
	long startTime = Platform::GetTimeInMillis();

	if (_particleRenderer)
	{
		FrameInfo frameInfo = _resultReader->GetThicknessBuffer(frameNum);

		float minThick = frameInfo.minThickness;
		float maxThick = frameInfo.maxThickness;
		float totDiffThick = maxThick - minThick;
		unsigned int numThicknessVals = frameInfo.bufferSize / sizeof(float);
		float* thicknessBuf = (float*)frameInfo.buffer;

		unsigned int colorBufSize = numThicknessVals * 3;
		char* colorBuf = (char*)malloc(colorBufSize);

		float* allColorsVecR = _colorBar->allColorsVecR;
		float* allColorsVecG = _colorBar->allColorsVecG;
		float* allColorsVecB = _colorBar->allColorsVecB;

		//TODO: 1000.0f should be taken from ColorBar class.
		float factor = 1000.0f / totDiffThick;

		for (int i = 0; i < numThicknessVals; i++)
		{
			int ii = i * 3;

			int index = (int)((thicknessBuf[i] - minThick) * factor);

			colorBuf[ii + 0] = allColorsVecR[index] * 255;
			colorBuf[ii + 1] = allColorsVecG[index] * 255;
			colorBuf[ii + 2] = allColorsVecB[index] * 255;
		}

		if (_particleRenderer)
		{
			//glm::vec3 carCenter = _assetsBuilder->GetSolid()->GetAABB().Center();
			//glm::vec3 particleCenter = _particleRenderer->GetBBoxCenter();

			//glm::vec3 delta = carCenter - particleCenter;

			_particleRenderer->UpdateColorBuffer(colorBuf, colorBufSize);
			//_particleRenderer->SetPosition(delta.x, delta.y, delta.z);
		}

		free(colorBuf);
		free(thicknessBuf);
	}

	Platform::debugPrint("\nTime for applying contour : %ld", Platform::GetTimeInMillis() - startTime);
}

char* ECoatPost::GetParticleColorBuf(int frameNum, unsigned int* bufSize)
{
	FrameInfo frameInfo = _resultReader->GetThicknessBuffer(frameNum);

	float minThick = frameInfo.minThickness;
	float maxThick = frameInfo.maxThickness;
	float totDiffThick = maxThick - minThick;
	unsigned int numThicknessVals = frameInfo.bufferSize / sizeof(float);
	float* thicknessBuf = (float*)frameInfo.buffer;

	unsigned int colorBufSize = numThicknessVals * 3;
	char* colorBuf = (char*)malloc(colorBufSize);

	float* allColorsVecR = _colorBar->allColorsVecR;
	float* allColorsVecG = _colorBar->allColorsVecG;
	float* allColorsVecB = _colorBar->allColorsVecB;

	//TODO: 1000.0f should be taken from ColorBar class.
	float factor = 1000.0f / totDiffThick;

	for (int i = 0; i < numThicknessVals; i++)
	{
		int index = (int)((thicknessBuf[i] - minThick) * factor);

		int ii = i * 3;
		colorBuf[ii + 0] = allColorsVecR[index] * 255;
		colorBuf[ii + 1] = allColorsVecG[index] * 255;
		colorBuf[ii + 2] = allColorsVecB[index] * 255;
	}

	free(thicknessBuf);
	bufSize[0] = colorBufSize;

	return colorBuf;
}

ECoatPost::~ECoatPost()
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