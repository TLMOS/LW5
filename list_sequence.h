#include <cstdlib>
#include <cstring>
#include "sequence.h"
#include "linked_list.h"
#include "iterator_interface.h"
#pragma once

template <class T>
class ListSequence : public Sequence<T>
{
private:
	LinkedList<T>* list_;
	class Iterator;
public:
	~ListSequence();

	ListSequence();
	ListSequence(T* items, size_t count);
	ListSequence(const LinkedList<T>& list);
	ListSequence(const Sequence<T>& sequence);

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
inline ListSequence<T>::~ListSequence()
{
	delete(list_);
}

template<class T>
inline ListSequence<T>::ListSequence()
{
	list_ = new LinkedList<T>();
}

template<class T>
inline ListSequence<T>::ListSequence(T* items, size_t count)
{
	list_ = new LinkedList<T>(items, count);
}

template<class T>
inline ListSequence<T>::ListSequence(const LinkedList<T>& list)
{
	list_ = new LinkedList<T>(list);
}

template<class T>
inline ListSequence<T>::ListSequence(const Sequence<T>& sequence)
{
	size_t length = sequence.GetLength();
	T* items = (T*)std::malloc(length * sizeof(T));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	for (size_t i = 0; i < length; i++)
		items[i] = sequence.Get(i);
	list_ = new LinkedList<T>(items, length);
	free(items);
}

template<class T>
inline T ListSequence<T>::GetFirst() const
{
	if (!list_->GetLength())
		throw std::out_of_range("Index out of range");

	return list_->GetFirst();
}

template<class T>
inline T ListSequence<T>::GetLast() const
{
	if (!list_->GetLength())
		throw std::out_of_range("Index out of range");

	return list_->GetLast();
}

template<class T>
inline T ListSequence<T>::Get(size_t index) const
{
	if (index < 0 || index >= list_->GetLength())
		throw std::out_of_range("Index out of range");

	return list_->Get(index);
}

template<class T>
inline bool ListSequence<T>::Contains(T item)
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
inline Sequence<T>* ListSequence<T>::GetSubsequence(size_t start_index, size_t end_index) const
{
	if (start_index < 0 || start_index >= list_->GetLength()
		|| end_index < 0 || end_index >= list_->GetLength())
		throw std::out_of_range("Index out of range");
	if (start_index > end_index)
		throw std::invalid_argument("End index should be greater than or equal to start index");

	ListSequence<T>* subSeq = new ListSequence<T>();
	delete(subSeq->list_);
	subSeq->list_ = list_->GetSubList(start_index, end_index);
	return subSeq;
}

template<class T>
inline size_t ListSequence<T>::GetLength() const
{
	return list_->GetLength();
}

template<class T>
inline void ListSequence<T>::Set(size_t index, T item)
{
	list_->Set(index, item);
}

template<class T>
inline void ListSequence<T>::Append(T item)
{
	list_->Append(item);
}

template<class T>
inline void ListSequence<T>::Prepend(T item)
{
	list_->Prepend(item);
}

template<class T>
inline void ListSequence<T>::InsertAt(T item, size_t index)
{
	if (index < 0 || index >= list_->GetLength())
		throw std::out_of_range("Index out of range");

	list_->InsertAt(item, index);
}

template<class T>
inline void ListSequence<T>::Delete(size_t index)
{
	list_->Delete(index);
}

template<class T>
inline Sequence<T>* ListSequence<T>::Concat(Sequence<T>* list) const
{
	ListSequence<T>* concatedSeq = new ListSequence<T>();
	delete(concatedSeq->list_);
	concatedSeq->list_ = new LinkedList<T>(*list_);
	for (int i = 0; i < list->GetLength(); i++)
		concatedSeq->Append(list->Get(i));
	
	return concatedSeq;
}

template<class T>
inline T& ListSequence<T>::operator[](size_t index)
{
	return (*list_)[index];
}

template<class T>
inline const T& ListSequence<T>::operator[](size_t index) const
{
	return (*list_)[index];
}

template<class T>
inline IIterator<T>* ListSequence<T>::GetIterator()
{
	return list_->GetIterator();
}
