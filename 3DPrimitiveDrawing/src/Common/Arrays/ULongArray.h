#ifndef ULongArray_H
#define ULongArray_H

class ULongArray
{
private:
	unsigned int _capacity;
	unsigned int _size;
	unsigned long* _arr;
	
	void ReCreateMem();

public:
	ULongArray(unsigned int capacity);
	~ULongArray();

	void push_back(unsigned long val);
	
	unsigned int size();
	unsigned int capacity();

	const unsigned long* getArray();
};

#endif
