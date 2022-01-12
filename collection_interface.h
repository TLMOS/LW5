#pragma once

template<class T>
class ICollection
{
public:
	virtual T GetFirst() const = 0;
	virtual T GetLast() const = 0;
	virtual T Get(size_t index) = 0;
	virtual size_t GetSize() const = 0;
	virtual const T& operator[](size_t index) = 0;
};
