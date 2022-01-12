#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#pragma once

template<class T>
class DynamicArray
{
private:
	T* items_;
	size_t size_;

public:
	~DynamicArray();

	DynamicArray();
	DynamicArray(size_t size);
	DynamicArray(T* items, size_t count);
	DynamicArray(const DynamicArray<T>& dynamicArray);

	T Get(size_t i) const;
	size_t GetSize() const;
	void Set(size_t i, T value);
	void Resize(size_t size);
	void Delete(size_t index);

	T& operator[](size_t ind);
	const T& operator[](size_t ind) const;
};

template<class T>
DynamicArray<T>::DynamicArray()
{
	items_ = NULL;
	size_ = 0;
}

template<class T>
DynamicArray<T>::DynamicArray(size_t size)
{
	if (size < 0)
		throw std::invalid_argument("Received negative size");

	items_ = (T*)std::malloc(size * sizeof(T));
	size_ = size;
}

template<class T>
DynamicArray<T>::DynamicArray(T* const items, size_t count)
{
	if (count < 0)
		throw std::invalid_argument("Received negative count");

	items_ = (T*)std::malloc(count * sizeof(T));
	if (!items_)
		throw std::runtime_error("Cannot allocate memory");
	std::memcpy(items_, items, count * sizeof(T));
	size_ = count;
}

template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& dynamicArray):
	DynamicArray(dynamicArray.items_, dynamicArray.size_) {}

template<class T>
DynamicArray<T>::~DynamicArray()
{
	free(items_);
}

template<class T>
T DynamicArray<T>::Get(size_t i) const
{
	if (i < 0 || i >= size_)
		throw std::out_of_range("Index out of range");

	return items_[i];
}

template<class T>
size_t DynamicArray<T>::GetSize() const
{
	return size_;
}

template<class T>
void DynamicArray<T>::Set(size_t i, T value)
{
	if (i < 0 || i >= size_)
		throw std::out_of_range("Index out of range");

	items_[i] = value;
}

template<class T>
void DynamicArray<T>::Resize(size_t newSize)
{
	if(newSize < 0)
		throw std::invalid_argument("Received negative size");

	T* tmp = (T*)malloc(newSize * sizeof(T));
	if (!tmp)
		throw std::runtime_error("Cannot allocate memory");
	memcpy(tmp, items_, std::min(size_, newSize) * sizeof(T));
	free(items_);
	items_ = tmp;
	size_ = newSize;
}

template<class T>
inline void DynamicArray<T>::Delete(size_t index)
{
	if (index < 0 || index >= size_)
		throw std::out_of_range("Index out of range");

	T* tmp = (T*)malloc((size_ - 1) * sizeof(T));
	if (!tmp)
		throw std::runtime_error("Cannot allocate memory");
	if(index > 0)
		memcpy(tmp, items_, index * sizeof(T));
	if(index < size_ - 1)
		memcpy(tmp + index, items_ + index + 1, (size_ - index - 1) * sizeof(T));
	free(items_);
	items_ = tmp;
	size_--;
}

template<class T>
T& DynamicArray<T>::operator[](size_t i)
{
	if (i < 0 || i >= size_)
		throw std::out_of_range("Index out of range");

	return items_[i];
}

template<class T>
const T& DynamicArray<T>::operator[](size_t i) const
{
	if (i < 0 || i >= size_)
		throw std::out_of_range("Index out of range");

	return items_[i];
}