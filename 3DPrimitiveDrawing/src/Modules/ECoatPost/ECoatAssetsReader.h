#ifndef ECoatAssetsReader_H
#define ECoatAssetsReader_H

#include <vector>
#include <string>

class ECoatAssetsReader
{
private:
	std::string _solid;
	std::string _fluid;
	std::vector<std::string> _sourcesVec;
	std::string _result;

public:
	ECoatAssetsReader(const std::string& filePath);
	~ECoatAssetsReader();

	std::string GetSolid();
	std::vector<std::string> GetSources();
	std::string GetFluid();
	std::string GetResult();
};

#endif