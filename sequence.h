#include "iterator_interface.h"
#pragma once

enum class SequenceType
{
	None = 1,
	Same,
	List,
	Array
};

template<class T>
class Sequence
{
public:
	virtual ~Sequence() {};
	
	virtual T GetFirst() const = 0;
	virtual T GetLast() const = 0;
	virtual T Get(size_t index) const = 0;
	virtual bool Contains(T item) = 0;
	virtual Sequence<T>* GetSubsequence(size_t start_index, size_t end_index) const = 0;
	virtual size_t GetLength() const = 0;
	
	virtual void Set(size_t index, T item) = 0;
	virtual void Append(T item) = 0;
	virtual void Prepend(T item) = 0;
	virtual void InsertAt(T item, size_t index) = 0;
	virtual void Delete(size_t index) = 0;
	virtual Sequence<T>* Concat(Sequence<T>* list) const = 0;

	virtual T& operator[](size_t index) = 0;
	virtual const T& operator[](size_t index) const = 0;

	virtual IIterator<T>* GetIterator() = 0;
};