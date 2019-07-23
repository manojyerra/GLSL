#ifndef VoidPtrArray_H
#define VoidPtrArray_H

class VoidPtrArray
{
private:
	unsigned int _capacity;
	int _size;
	void** _arr;
	
	void ReCreateMem();

public:
	VoidPtrArray(unsigned int capacity);
	~VoidPtrArray();

	void push_back(void* val);
	
	unsigned int size();
	unsigned int capacity();

	void** getArray();
};

#endif
