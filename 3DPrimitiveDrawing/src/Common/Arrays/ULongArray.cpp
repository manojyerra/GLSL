#include "ULongArray.h"
#include <string>

ULongArray::ULongArray(unsigned int capacity)
{
	_capacity = capacity;
	_size = 0;

	_arr = new unsigned long[_capacity];
}

void ULongArray::ReCreateMem()
{
	int newCapacity = 3 * _capacity;
	unsigned long* newArr = new unsigned long[newCapacity];
	memcpy(newArr, _arr, _capacity*8);
	delete[] _arr;
	_arr = newArr;
	_capacity = newCapacity;
}

void ULongArray::push_back(unsigned long val)
{
	if (_size + 1 >= _capacity)
		ReCreateMem();

	_arr[_size] = val;
	_size++;
}

unsigned int ULongArray::size()
{
	return _size;
}

unsigned int ULongArray::capacity()
{
	return _capacity;
}

const unsigned long* ULongArray::getArray()
{
	unsigned long first = _arr[0];
	return (unsigned long*)_arr;
}

ULongArray::~ULongArray()
{
	if(_arr)
	{
		delete[] _arr;
		_arr = NULL;
	}
}