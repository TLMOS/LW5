#include <cstdlib>
#include <cstring>
#include "sequence.h"
#include "dynamic_array.h"
#pragma once

template <class T>
class ArraySequence : public Sequence<T>
{
private:
	DynamicArray<T>* array_;
	size_t length_;
	class Iterator;

public:
	~ArraySequence();

	ArraySequence();
	ArraySequence(T* items, size_t count);
	ArraySequence(const DynamicArray<T>& array);
	ArraySequence(const Sequence<T>& sequence);

	virtual T GetFirst() const;
	virtual T GetLast() const;
	virtual T Get(size_t index) const;
	virtual bool Contains(T item);
	virtual Sequence<T>* GetSubsequence(size_t start_index, size_t end_index) const;
	virtual size_t GetLength() const;

	virtual void Set(size_t index, T item);
	virtual void Append(T item);
	virtual void Prepend(T item);
	virtual void InsertAt(T item, size_t index);
	virtual void Delete(size_t index);
	virtual Sequence<T>* Concat(Sequence<T>* list) const;

	virtual T& operator[](size_t index);
	virtual const T& operator[](size_t index) const;

	virtual IIterator<T>* GetIterator();
};

template<class T>
inline ArraySequence<T>::~ArraySequence()
{
	delete(array_);
}

template<class T>
inline ArraySequence<T>::ArraySequence()
{
	array_ = new DynamicArray<T>(0);
	length_ = 0;
}

template<class T>
inline ArraySequence<T>::ArraySequence(T* items, size_t count)
{
	array_ = new DynamicArray<T>(items, count);
	length_ = count;
}

template<class T>
inline ArraySequence<T>::ArraySequence(const DynamicArray<T>& array)
{
	array_ = new DynamicArray<T>(array);
	length_ = array.GetSize();
}

template<class T>
inline ArraySequence<T>::ArraySequence(const Sequence<T>& sequence)
{
	length_ = sequence.GetLength();
	T* items = (T*)std::malloc(length_ * sizeof(T));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	for (size_t i = 0; i < length_; i++)
		items[i] = sequence.Get(i);
	array_ = new DynamicArray<T>(items, length_);
	free(items);
}

template<class T>
inline T ArraySequence<T>::GetFirst() const
{
	if (!length_)
		throw std::out_of_range("Index out of range");

	return array_->Get(0);
}

template<class T>
inline T ArraySequence<T>::GetLast() const
{
	if (!length_)
		throw std::out_of_range("Index out of range");

	return array_->Get(length_ - 1);
}

template<class T>
inline T ArraySequence<T>::Get(size_t index) const
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	return array_->Get(index);
}

template<class T>
inline bool ArraySequence<T>::Contains(T item)
{
	IIterator<T>* it = GetIterator();
	for (; it->HasNext(); it->Next())
	{
		if (it->GetCurrentItem() == item)
		{
			delete(it);
			return true;
		}
	}
	delete(it);
	return false;
}

template<class T>
inline Sequence<T>* ArraySequence<T>::GetSubsequence(size_t start_index, size_t end_index) const
{
	if (start_index < 0 || start_index >= length_
		|| end_index < 0 || end_index >= length_)
		throw std::out_of_range("Index out of range");
	if (start_index > end_index)
		throw std::invalid_argument("End index should be greater than or equal to start index");

	size_t length = end_index - start_index + 1;
	T* items = (T*)std::malloc(length * sizeof(T));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	for (size_t i = 0; i < length; i++)
		items[i] = array_->Get(start_index + i);
	ArraySequence<T>* subseq = new ArraySequence<T>(items, length);
	free(items);
	return subseq;
}

template<class T>
inline size_t ArraySequence<T>::GetLength() const
{
	return length_;
}

template<class T>
inline void ArraySequence<T>::Set(size_t index, T item)
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	array_->Set(index, item);
}

template<class T>
inline void ArraySequence<T>::Append(T item)
{
	if (array_->GetSize() == 0)
		array_->Resize(1);
	while (length_ + 1 > array_->GetSize())
		array_->Resize(array_->GetSize() * 2);

	array_->Set(length_, item);
	length_++;
}

template<class T>
inline void ArraySequence<T>::Prepend(T item)
{
	if (array_->GetSize() == 0)
		array_->Resize(1);
	while (length_ + 1 > array_->GetSize())
		array_->Resize(array_->GetSize() * 2);

	for (size_t i = length_; i > 0; i--)
		array_->Set(i, array_->Get(i - 1));
	array_->Set(0, item);
	length_++;
}

template<class T>
inline void ArraySequence<T>::InsertAt(T item, size_t index)
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	if (array_->GetSize() == 0)
		array_->Resize(1);
	while (length_ + 1 > array_->GetSize())
		array_->Resize(array_->GetSize() * 2);

	for (size_t i = length_; i > index; i--)
		array_->Set(i, array_->Get(i - 1));
	array_->Set(index, item);
	length_++;

}

template<class T>
inline void ArraySequence<T>::Delete(size_t index)
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	array_->Delete(index);
	length_--;
	if (array_->GetSize() / 2 >= length_)
		array_->Resize(array_->GetSize() / 2);
}

template<class T>
inline Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* list) const
{
	size_t length = length_ + list->GetLength();
	T* items = (T*)std::malloc(length * sizeof(T));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	for (size_t i = 0; i < length_; i++)
		items[i] = array_->Get(i);
	for (size_t i = 0; i < list->GetLength(); i++)
		items[length_ + i] = list->Get(i);
	ArraySequence<T>* concated = new ArraySequence<T>(items, length);
	free(items);
	return concated;
}

template<class T>
inline T& ArraySequence<T>::operator[](size_t index)
{
	return (*array_)[index];
}

template<class T>
inline const T& ArraySequence<T>::operator[](size_t index) const
{
	return (*array_)[index];
}

template<class T>
inline IIterator<T>* ArraySequence<T>::GetIterator()
{
	return (IIterator<T>*)(new Iterator(this));
}

template<class T>
class ArraySequence<T>::Iterator : IIterator<T> {
	friend class ArraySequence<T>;
private:
	ArraySequence<T>* array_;
	size_t count_;
public:
	Iterator(ArraySequence<T>* array)
	{
		array_ = array;
		count_ = 0;
	}
	virtual T GetCurrentItem()
	{
		return array_->Get(count_);
	}
	virtual size_t GetCurrentIndex()
	{
		return count_;
	}
	virtual bool HasNext()
	{
		return count_ < array_->GetLength();
	}
	bool Next()
	{
		count_++;
		return true;
	}
};