#ifndef UIntArray_H
#define UIntArray_H

class UIntArray
{
private:
	unsigned int _capacity;
	unsigned int _size;
	
	void ReCreateMem();
	unsigned int* _arr;

public:

	UIntArray(unsigned int capacity);
	~UIntArray();

	void push_back(unsigned int val);
	
	unsigned int size();
	unsigned int capacity();

	const unsigned int* getArray();
	void print();
};

#endif
