#ifndef ContourMap_H
#define ContourMap_H

class ContourMap
{
private:

public:
	ContourMap(float* stlVertexArr, unsigned int stlVertexCount, 
				unsigned char* stlColorBuf, unsigned int stlColorBufLen, 
				float* parVertexArr, unsigned int parVertexArrSize, 
				unsigned char* parColorBuf, unsigned int parColorBufLen);

	~ContourMap();
};

#endif
