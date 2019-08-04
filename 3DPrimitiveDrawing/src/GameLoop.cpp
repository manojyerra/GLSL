#include "GameLoop.h"
#include "GLMemory.h"
#include "ShaderProgramsManager.h"
#include "Input.h"
#include "Cam.h"
#include "Cam2D.h"

GameLoop::GameLoop(float sw, float sh)
{	
	_sw = sw;
	_sh = sh;

	_zNear = 0.15f;
	_zFar = 1000000.0f;
	_zNearPlaneW = 0.25f;

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
	ShaderProgramsManager::GetInstance()->DeleteInstance();
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


/*

#include <H5Cpp.h>


	float* _vertexData = nullptr;
	unsigned int _vertexCount = 0;

	unsigned int rows = 0;
	unsigned int cols = 0;

	string filePath = "data/result_fine.ecoat";

	if (!H5::H5File::isHdf5(filePath.c_str()))
	{
		printf("Invalid H5File");
	}

	H5::H5File* h5File = new H5::H5File(filePath.c_str(), H5F_ACC_RDWR);

	H5::Group stepsGroup = h5File->openGroup("Workpiece");

	if (H5::IdComponent::isValid(stepsGroup.getId()))
	{
		int totSteps = stepsGroup.getNumObjs();

		//H5::Group singleStepGroup = stepsGroup.openGroup("1");
		H5::Group particleGroup = stepsGroup.openGroup("Particle");
		H5::DataSet dataSet = particleGroup.openDataSet("position");

		H5T_class_t type_class = dataSet.getTypeClass();

		if (type_class == H5T_FLOAT)
		{
			H5::FloatType floatType = dataSet.getFloatType();
			size_t floatSize = floatType.getSize();
		}

		H5::DataSpace dataSpace = dataSet.getSpace();
		int numDimentions = dataSpace.getSimpleExtentNdims();

		hsize_t arrInfo[2];
		int ndims = dataSpace.getSimpleExtentDims(arrInfo, NULL);
		rows = arrInfo[0];
		cols = arrInfo[1];

		hsize_t dimsm[2];
		dimsm[0] = rows;
		dimsm[1] = cols;
		H5::DataSpace memSpace(numDimentions, dimsm);

		_vertexData = new float[rows * cols];
		_vertexCount = rows;

		dataSet.read(_vertexData, H5::PredType::NATIVE_FLOAT, memSpace, dataSpace);

		memSpace.close();
		dataSpace.close();
		dataSet.close();
		particleGroup.close();
		//singleStepGroup.close();
	}

	stepsGroup.close();
	h5File->close();

*/