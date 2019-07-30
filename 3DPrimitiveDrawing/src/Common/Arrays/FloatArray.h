#ifndef FloatArray_H
#define FloatArray_H

#include "glm/glm.hpp"

class FloatArray
{
private:
	unsigned int _capacity;
	unsigned int _size;
	float* _arr;

	void ReCreateMem();
	void ReCreateMem(unsigned int newCapacity);

public:
	FloatArray();
	FloatArray(unsigned int capacity);
	~FloatArray();

	void push_back(float val);
	void push_back_2(float v1, float v2);
	void push_back_3(float v1, float v2, float v3);

	void push_back_2(const glm::vec3& vec);
	void push_back_3(const glm::vec3& vec);
	
	unsigned int size();
	unsigned int capacity();

	void addCapacity(unsigned int addCapacity);

	const float* getArray();
};

#endif
