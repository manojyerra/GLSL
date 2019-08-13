#include "ECoatResultReader.h"

ECoatResultReader::ECoatResultReader(std::string resultFilePath)
{
	if (!H5::H5File::isHdf5(resultFilePath.c_str()))
	{
		throw new std::exception("Exception : Invalid H5File");
	}

	_h5File = new H5::H5File(resultFilePath.c_str(), H5F_ACC_RDWR);
}

unsigned int ECoatResultReader::GetFrameCount()
{
	H5::Group stepsGroup = _h5File->openGroup("Steps");
	return stepsGroup.getNumObjs();
}

FrameInfo ECoatResultReader::GetParticleBuffer(unsigned int frameNum)
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

	FrameInfo frameInfo;
	frameInfo.buffer = (char*)vertexData;
	frameInfo.bufferSize = (unsigned int)rows * cols * sizeof(float);
	frameInfo.rot.x = ReadFloatAttributes(&singleStepGroup, "rotation-x");
	frameInfo.rot.y = ReadFloatAttributes(&singleStepGroup, "rotation-y");
	frameInfo.rot.z = ReadFloatAttributes(&singleStepGroup, "rotation-z");
	frameInfo.trans.x = ReadFloatAttributes(&singleStepGroup, "translation-x");
	frameInfo.trans.y = ReadFloatAttributes(&singleStepGroup, "translation-y");
	frameInfo.trans.z = ReadFloatAttributes(&singleStepGroup, "translation-z");
	frameInfo.cellSize.x = ReadFloatAttributes(&singleStepGroup, "cellsize-x");
	frameInfo.cellSize.y = ReadFloatAttributes(&singleStepGroup, "cellsize-y");
	frameInfo.cellSize.z = ReadFloatAttributes(&singleStepGroup, "cellsize-z");
	frameInfo.minThickness = ReadFloatAttributes(&singleStepGroup, "min-thickness");
	frameInfo.maxThickness = ReadFloatAttributes(&singleStepGroup, "max-thickness");

	memSpace.close();
	dataSpace.close();
	dataSet.close();
	particleGroup.close();
	singleStepGroup.close();

	return frameInfo;
}

FrameInfo ECoatResultReader::GetParticleColorBuffer(unsigned int frameNum)
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

	FrameInfo frameInfo;
	frameInfo.buffer = (char*)thicknessBuf;
	frameInfo.bufferSize = (unsigned int)memSize;
	frameInfo.rot.x = ReadFloatAttributes(&singleStepGroup, "rotation-x");
	frameInfo.rot.y = ReadFloatAttributes(&singleStepGroup, "rotation-y");
	frameInfo.rot.z = ReadFloatAttributes(&singleStepGroup, "rotation-z");
	frameInfo.trans.x = ReadFloatAttributes(&singleStepGroup, "translation-x");
	frameInfo.trans.y = ReadFloatAttributes(&singleStepGroup, "translation-y");
	frameInfo.trans.z = ReadFloatAttributes(&singleStepGroup, "translation-z");
	frameInfo.cellSize.x = ReadFloatAttributes(&singleStepGroup, "cellsize-x");
	frameInfo.cellSize.y = ReadFloatAttributes(&singleStepGroup, "cellsize-y");
	frameInfo.cellSize.z = ReadFloatAttributes(&singleStepGroup, "cellsize-z");
	frameInfo.minThickness = ReadFloatAttributes(&singleStepGroup, "min-thickness");
	frameInfo.maxThickness = ReadFloatAttributes(&singleStepGroup, "max-thickness");

	memSpace.close();
	dataSpace.close();
	dataSet.close();
	particleGroup.close();
	singleStepGroup.close();

	return frameInfo;
}

char* ECoatResultReader::GetParticleBufferWorkpiece(unsigned int frameNum, unsigned int* dataSize)
{
	return GetParticleBufferWorkpiece(frameNum, dataSize, H5::PredType::NATIVE_FLOAT);
}

char* ECoatResultReader::GetParticleBufferWorkpiece(unsigned int frameNum, unsigned int* dataSize, H5::PredType dataType)
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

	//size_t floatSize = dataSet.getFloatType().getSize();
	if (dataSet.getTypeClass() != H5T_FLOAT)
	{
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

	unsigned int vertexBufSize = 0;

	if (dataType == H5::PredType::NATIVE_FLOAT)
	{
		vertexBufSize = rows * cols * sizeof(float);
	}
	else if(dataType == H5::PredType::NATIVE_DOUBLE)
	{
		vertexBufSize = rows * cols * sizeof(double);
	}
	else
	{
		throw new std::exception("Exception: Unsupported data type.");
	}

	char* vertexData = (char*)malloc(vertexBufSize);
	dataSet.read(vertexData, dataType, memSpace, dataSpace);

	memSpace.close();
	dataSpace.close();
	dataSet.close();
	particleGroup.close();
	workPieceGroup.close();

	dataSize[0] = vertexBufSize;

	return vertexData;
}

float ECoatResultReader::ReadFloatAttributes(H5::Group* group, const std::string& attr_name)
{
	double value;

	if (group->attrExists(attr_name.c_str()))
	{
		// get the dataspace associated with the attribute
		H5::Attribute attr = group->openAttribute(attr_name.c_str());
		H5::DataSpace dataspace = attr.getSpace();
		attr.read(attr.getDataType(), &value);
		dataspace.close();
		attr.close();
	}

	return static_cast<float>(value);
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
