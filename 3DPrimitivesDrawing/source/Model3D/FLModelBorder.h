#ifndef FLModelBorder_H
#define FLModelBorder_H

#include "DefinesAndIncludes.h"
#include "GLUtil/GL2DState.h"
#include "Shapes2D/Rect2D.h"
#include <vector>
#include <string>
using namespace std;

#define U_INT unsigned int

class FLModelBorder
{
private:
	vector<float> _borderVec;
	GL2DState state2D;

public:
	FLModelBorder();
	~FLModelBorder();

	void CalcBorder(float* verArr, U_INT  numIndices, U_INT  indicesType, unsigned char* indicesPointer, float* mat, Rect2D* rect);
	void DrawBorder();
};

#endif
