#include "UIntArray.h"
#include <string>

UIntArray::UIntArray(unsigned int capacity)
{
	_capacity = capacity;
	_size = 0;

	_arr = new unsigned int[_capacity];
}

void UIntArray::ReCreateMem()
{
	int newCapacity = 2 * _capacity;
	unsigned int* newArr = new unsigned int[newCapacity];
	memcpy(newArr, _arr, _capacity*4);
	delete[] _arr;
	_arr = newArr;
	_capacity = newCapacity;
}

void UIntArray::push_back(unsigned int val)
{
	if (_size + 1 >= _capacity)
		ReCreateMem();

	_arr[_size] = val;

	_size++;
}

unsigned int UIntArray::get(int index)
{
	return _arr[index];
}

unsigned int UIntArray::size()
{
	return _size;
}

unsigned int UIntArray::capacity()
{
	return _capacity;
}

void UIntArray::clear()
{
	_size = 0;
}

const unsigned int* UIntArray::getArray()
{
	return (unsigned int*)_arr;
}

void UIntArray::print()
{
	for (int i = 0; i < _size; i++)
	{
		printf("\n%d", _arr[i]);
	}
}

UIntArray::~UIntArray()
{
	if(_arr)
	{
		delete[] _arr;
		_arr = NULL;
	}
}