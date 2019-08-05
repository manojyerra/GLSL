#include "ECoatResultReader.h"

ECoatResultReader::ECoatResultReader(std::string resultFilePath)
{
	if (!H5::H5File::isHdf5(resultFilePath.c_str()))
	{
		throw new std::exception("Exception : Invalid H5File");
	}

	_h5File = new H5::H5File(resultFilePath.c_str(), H5F_ACC_RDWR);	
}

char* ECoatResultReader::GetParticleBuffer(unsigned int frameNum, unsigned int* dataSize)
{
	H5::Group stepsGroup = _h5File->openGroup("Steps");

	if (!H5::IdComponent::isValid(stepsGroup.getId()))
	{
		throw new std::exception("Exception : Invalid Group");
	}

	if (frameNum <= 0 || frameNum > stepsGroup.getNumObjs())
	{
		throw new std::exception("Exception : Invalid frame number");
	}

	std::string frameNumStr = std::to_string(frameNum);
	H5::Group singleStepGroup = stepsGroup.openGroup(frameNumStr.c_str());
	H5::Group particleGroup = singleStepGroup.openGroup("Particle");
	H5::DataSet dataSet = particleGroup.openDataSet("position");

	if (dataSet.getTypeClass() != H5T_FLOAT)
	{
		//size_t floatSize = dataSet.getFloatType().getSize();
		throw new std::exception("Exception : Invalid dataset type");
	}

	H5::DataSpace dataSpace = dataSet.getSpace();
	int numDimentions = dataSpace.getSimpleExtentNdims();

	if (numDimentions != 2)
	{
		throw new std::exception("Exception : Unsupported array dimention");
	}

	hsize_t arrInfo[2];
	int ndims = dataSpace.getSimpleExtentDims(arrInfo, NULL);
	unsigned int rows = arrInfo[0];
	unsigned int cols = arrInfo[1];

	H5::DataSpace memSpace(numDimentions, arrInfo);

	float* vertexData = new float[rows * cols];

	dataSet.read(vertexData, H5::PredType::NATIVE_FLOAT, memSpace, dataSpace);

	memSpace.close();
	dataSpace.close();
	dataSet.close();
	particleGroup.close();
	singleStepGroup.close();

	dataSize[0] = rows*cols*sizeof(float);

	return (char*)vertexData;
}

char* ECoatResultReader::GetParticleColorBuffer(unsigned int frameNum, unsigned int* dataSize)
{
	H5::Group stepsGroup = _h5File->openGroup("Steps");

	if (!H5::IdComponent::isValid(stepsGroup.getId()))
	{
		throw new std::exception("Exception : Invalid Group");
	}

	if (frameNum <= 0 || frameNum > stepsGroup.getNumObjs())
	{
		throw new std::exception("Exception : Invalid frame number");
	}

	std::string frameNumStr = std::to_string(frameNum);
	H5::Group singleStepGroup = stepsGroup.openGroup(frameNumStr.c_str());
	H5::Group particleGroup = singleStepGroup.openGroup("CoatingThickness");
	H5::DataSet dataSet = particleGroup.openDataSet("thickness");

	if (dataSet.getTypeClass() != H5T_FLOAT)
	{
		//size_t floatSize = dataSet.getFloatType().getSize();
		throw new std::exception("Exception : Invalid dataset type");
	}

	H5::DataSpace dataSpace = dataSet.getSpace();
	int numDimentions = dataSpace.getSimpleExtentNdims();

	if (numDimentions != 2)
	{
		throw new std::exception("Exception : Unsupported array dimention");
	}

	hsize_t arrInfo[2];
	int ndims = dataSpace.getSimpleExtentDims(arrInfo, NULL);
	unsigned int rows = arrInfo[0];

	H5::DataSpace memSpace(numDimentions, arrInfo);

	unsigned int memSize = rows * sizeof(float);
	char* thicknessBuf = (char*)malloc(memSize);

	dataSet.read(thicknessBuf, H5::PredType::NATIVE_FLOAT, memSpace, dataSpace);

	memSpace.close();
	dataSpace.close();
	dataSet.close();
	particleGroup.close();
	singleStepGroup.close();

	dataSize[0] = memSize;

	return thicknessBuf;
}

char* ECoatResultReader::GetParticleBufferWorkpiece(unsigned int frameNum, unsigned int* dataSize)
{
	H5::Group workPieceGroup = _h5File->openGroup("Workpiece");

	if (!H5::IdComponent::isValid(workPieceGroup.getId()))
	{
		throw new std::exception("Exception : Invalid Group");
	}

	if (frameNum <= 0 || frameNum > workPieceGroup.getNumObjs())
	{
		throw new std::exception("Exception : Invalid frame number");
	}

	H5::Group particleGroup = workPieceGroup.openGroup("Particle");
	H5::DataSet dataSet = particleGroup.openDataSet("position");

	if (dataSet.getTypeClass() != H5T_FLOAT)
	{
		//size_t floatSize = dataSet.getFloatType().getSize();
		throw new std::exception("Exception : Invalid dataset type");
	}

	H5::DataSpace dataSpace = dataSet.getSpace();
	int numDimentions = dataSpace.getSimpleExtentNdims();

	if (numDimentions != 2)
	{
		throw new std::exception("Exception : Unsupported array dimention");
	}

	hsize_t arrInfo[2];
	int ndims = dataSpace.getSimpleExtentDims(arrInfo, NULL);
	unsigned int rows = arrInfo[0];
	unsigned int cols = arrInfo[1];

	H5::DataSpace memSpace(numDimentions, arrInfo);

	char* vertexData = (char*)malloc(rows * cols * sizeof(float));

	dataSet.read(vertexData, H5::PredType::NATIVE_FLOAT, memSpace, dataSpace);

	memSpace.close();
	dataSpace.close();
	dataSet.close();
	particleGroup.close();
	workPieceGroup.close();

	dataSize[0] = rows * cols * sizeof(float);

	return vertexData;
}

ECoatResultReader::~ECoatResultReader()
{
	if (_h5File)
	{
		_h5File->close();
		delete _h5File;
		_h5File = NULL;
	}
}
