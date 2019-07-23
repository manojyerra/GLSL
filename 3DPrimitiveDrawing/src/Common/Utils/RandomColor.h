#ifndef RandomColor_H
#define RandomColor_H

#include <vector>

class RandomColor
{
private:
	static bool init;
	static std::vector<unsigned int> colorVec;
	int index;

public:
	RandomColor();
	unsigned int NextColor();
	void Reset();
	~RandomColor();
};

#endif
