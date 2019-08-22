#ifndef BufferInfo_H
#define BufferInfo_H

class BufferInfo
{
public:
	char* buffer;
	long size;

	BufferInfo()
	{
		buffer = nullptr;
		size = 0;
	}

	BufferInfo(char* buffer, long size)
	{
		this->buffer = buffer;
		this->size = size;
	}

	bool IsEmpty()
	{
		return (buffer == nullptr || size == 0);
	}

	bool HasData()
	{
		return (buffer != nullptr && size > 0);
	}
};

#endif