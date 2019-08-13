#include "ECoatAssetsReader.h"
#include "json/json.h"
#include "json/reader.h"
#include <fstream>

ECoatAssetsReader::ECoatAssetsReader(const std::string& filePath)
{
	_solid = "";
	_fluid = "";
	_sourcesVec.clear();
 	_result = "";

	std::ifstream input(filePath);
	
	Json::Reader reader;
	Json::Value root;
	reader.parse(input, root);

	Json::Value solidValArr = root["Solids"];

	if (solidValArr.isArray())
	{
		if (solidValArr.size() > 1)
			throw new std::exception("Exception: Unsupported number of solids.");

		if (solidValArr.size() == 1)
			_solid = solidValArr[(unsigned int)0].asString();
	}

	Json::Value fluidValArr = root["Fluids"];

	if (fluidValArr.isArray())
	{
		if (fluidValArr.size() > 1)
			throw new std::exception("Exception: Unsupported number of fluids.");

		if (fluidValArr.size() == 1)
			_fluid = fluidValArr[(unsigned int)0].asString();
	}

	Json::Value result = root["Result"];

	if (!result.isString())
		throw new std::exception("Exception: Result file path not found.");

	_result = result.asString();


	Json::Value sourcesArr = root["Sources"];
	
	if (sourcesArr.isArray())
	{
		for (unsigned int i = 0; i < sourcesArr.size(); i++) {
			_sourcesVec.push_back(sourcesArr[i].asString());
		}
	}
}

std::string ECoatAssetsReader::GetSolid()
{
	return _solid;
}
std::vector<std::string> ECoatAssetsReader::GetSources()
{
	return _sourcesVec;
}
std::string ECoatAssetsReader::GetFluid()
{
	return _fluid;
}
std::string ECoatAssetsReader::GetResult()
{
	return _result;
}

ECoatAssetsReader::~ECoatAssetsReader()
{

}