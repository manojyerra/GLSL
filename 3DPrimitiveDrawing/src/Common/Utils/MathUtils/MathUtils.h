#ifndef MathUtils_H
#define MathUtils_H

#define DEG_RAD 0.0174532925194f
#define RAD_DEG 57.295780490443f
#define PI_VAL 3.1415926f

#define minVal(a,b) (a < b) ? a : b
#define maxVal(a,b) (a > b) ? a : b

class MathUtils
{
public:
	static float MinMax(float val, float minLimit, float maxLimit)
	{
		if (val < minLimit)
			val = minLimit;

		if (val > maxLimit)
			val = maxLimit;

		return val;
	}
};

#endif