#ifndef PNG_READ_WRITE_H
#define PNG_READ_WRITE_H

#include <string>

class PNGReadWrite
{
public:
	static unsigned char* Read(std::string path, int* imgWidth, int* imgHeight, int* bytesPerPixel);
	static bool Write(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath);

	static bool BeginWrite(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath);
	static int WritePercent(float percent);
	static void EndWrite();
};

#endif
