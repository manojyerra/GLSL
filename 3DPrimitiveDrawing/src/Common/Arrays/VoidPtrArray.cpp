#include "VoidPtrArray.h"
#include <string>

VoidPtrArray::VoidPtrArray(unsigned int capacity)
{
	_capacity = capacity;
	_size = 0;

	_arr = new void*[_capacity];
}

void VoidPtrArray::ReCreateMem()
{
	int newCapacity = 3 * _capacity;
	void** newArr = new void*[newCapacity];

	memcpy((void*)newArr, (void*)_arr, _capacity* sizeof(void*));
	delete[] _arr;
	_arr = newArr;
	_capacity = newCapacity;
}

void VoidPtrArray::push_back(void* val)
{
	if (_size + 1 >= _capacity)
		ReCreateMem();

	_arr[_size] = val;
	_size++;
}

unsigned int VoidPtrArray::size()
{
	return _size;
}

unsigned int VoidPtrArray::capacity()
{
	return _capacity;
}

void** VoidPtrArray::getArray()
{
	return (void**)_arr;
}

VoidPtrArray::~VoidPtrArray()
{
	if(_arr)
	{
		delete[] _arr;
		_arr = NULL;
	}
}